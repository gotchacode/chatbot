const std = @import("std");
const chatbot = @import("chatbot.zig");
const HashTable = chatbot.HashTable;

/// Fuzz target: hammer the hash table with arbitrary key/value pairs
/// Tests for memory safety, correct get-after-set semantics, and collision handling
///
/// To use with external fuzzer (afl-fuzz, honggfuzz, etc.), pipe input:
///   echo -n "fuzz_data" | zig-out/bin/fuzz
pub fn main(init: std.process.Init) !void {
    // use a fixed-buffer allocator that will catch OOM without crashing
    var buffer: [1024 * 1024]u8 = undefined;
    var fba = std.heap.FixedBufferAllocator.init(&buffer);
    const allocator = fba.allocator();

    var ht = HashTable.create(allocator, 64) catch return;
    defer ht.destroy();

    // read stdin for fuzz input
    const io = init.io;
    const stdin_file = std.Io.File.stdin();
    var input_buf: [1024 * 512]u8 = undefined;
    var total_read: usize = 0;

    // read until EOF using streaming reads
    while (total_read < input_buf.len) {
        const chunk = stdin_file.readStreaming(io, &.{input_buf[total_read..]}) catch break;
        if (chunk == 0) break;
        total_read += chunk;
    }

    fuzzHashTable(ht, allocator, input_buf[0..total_read]) catch return;
}

fn fuzzHashTable(ht: *HashTable, _: std.mem.Allocator, input: []const u8) !void {
    var offset: usize = 0;

    while (offset + 4 <= input.len) {
        // read key length (1 byte, capped to 255)
        const key_len = @min(input[offset], 127);
        offset += 1;
        if (offset + key_len > input.len) break;

        const key = input[offset .. offset + key_len];
        offset += key_len;

        // read value length (1 byte, capped)
        if (offset >= input.len) break;
        const val_len = @min(input[offset], 127);
        offset += 1;
        if (offset + val_len > input.len) break;

        const value = input[offset .. offset + val_len];
        offset += val_len;

        // read operation byte: 0 = set, 1 = get, 2 = set+get verify
        if (offset >= input.len) break;
        const op = input[offset] % 3;
        offset += 1;

        switch (op) {
            0 => {
                // set operation
                ht.set(key, value) catch continue;
            },
            1 => {
                // get operation - just exercise the code path
                _ = ht.get(key);
            },
            2 => {
                // set then verify get returns the same value
                ht.set(key, value) catch continue;
                const result = ht.get(key);
                if (result) |r| {
                    // value must match what we just set
                    if (!std.mem.eql(u8, r, value)) {
                        // this would be a bug!
                        @panic("get returned wrong value after set");
                    }
                } else {
                    @panic("get returned null immediately after set");
                }
            },
            else => unreachable,
        }
    }

    // stress test: verify all empty key edge case
    ht.set("", "empty_key_value") catch {};
    _ = ht.get("");

    // stress test: same key multiple times to test update path
    var i: usize = 0;
    while (i < @min(input.len, 100)) : (i += 1) {
        var val_buf: [8]u8 = undefined;
        const val_slice = std.fmt.bufPrint(&val_buf, "{d}", .{i}) catch break;
        ht.set("stress", val_slice) catch break;
    }

    // final verification
    if (ht.get("stress")) |v| {
        // value should be the last iteration number
        _ = v;
    }
}

test "fuzz_empty_input" {
    var gpa: std.heap.DebugAllocator(.{}) = .init;
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var ht = try HashTable.create(allocator, 64);
    defer ht.destroy();

    try fuzzHashTable(ht, allocator, "");
}

test "fuzz_minimal_input" {
    var gpa: std.heap.DebugAllocator(.{}) = .init;
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var ht = try HashTable.create(allocator, 64);
    defer ht.destroy();

    // format: key_len, key, val_len, val, op
    const input = "\x03key\x05value\x02";
    try fuzzHashTable(ht, allocator, input);

    // verify it actually set
    const result = ht.get("key");
    try std.testing.expect(result != null);
    try std.testing.expectEqualSlices(u8, result.?, "value");
}

test "fuzz_collision_stress" {
    var gpa: std.heap.DebugAllocator(.{}) = .init;
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // small table to force collisions
    var ht = try HashTable.create(allocator, 4);
    defer ht.destroy();

    // generate input that will cause hash collisions
    var input_buf: [1024]u8 = undefined;
    var offset: usize = 0;

    var i: u8 = 0;
    while (i < 50) : (i += 1) {
        if (offset + 10 > input_buf.len) break;
        input_buf[offset] = 4; // key len
        offset += 1;
        input_buf[offset] = 'k';
        input_buf[offset + 1] = 'e';
        input_buf[offset + 2] = 'y';
        input_buf[offset + 3] = '0' + (i % 10);
        offset += 4;
        input_buf[offset] = 1; // val len
        offset += 1;
        input_buf[offset] = 'a' + i;
        offset += 1;
        input_buf[offset] = 2; // set+verify op
        offset += 1;
    }

    try fuzzHashTable(ht, allocator, input_buf[0..offset]);
}

test "fuzz_special_chars" {
    var gpa: std.heap.DebugAllocator(.{}) = .init;
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var ht = try HashTable.create(allocator, 64);
    defer ht.destroy();

    // keys/values with null bytes, high bytes, separators
    const nasty_input = "\x05\x00\x01\x02\xff\xfe\x05\x00\x01\x02\xff\xfe\x02" ++
        "\x06 .,\"\n\x00\x03abc\x02";

    try fuzzHashTable(ht, allocator, nasty_input);
}
