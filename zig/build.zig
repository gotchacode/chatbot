const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "chat",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/main.zig"),
            .target = target,
            .optimize = optimize,
        }),
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the chatbot");
    run_step.dependOn(&run_cmd.step);

    const tests = b.addTest(.{
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/chatbot.zig"),
            .target = target,
            .optimize = optimize,
        }),
    });

    const run_test = b.addRunArtifact(tests);
    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_test.step);

    // Fuzz tests
    const fuzz_tests = b.addTest(.{
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/fuzz.zig"),
            .target = target,
            .optimize = optimize,
        }),
    });

    const run_fuzz_test = b.addRunArtifact(fuzz_tests);
    const fuzz_step = b.step("fuzz", "Run fuzz tests");
    fuzz_step.dependOn(&run_fuzz_test.step);

    // Fuzz executable for use with external fuzzers (afl-fuzz, etc.)
    const fuzz_exe = b.addExecutable(.{
        .name = "fuzz",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/fuzz.zig"),
            .target = target,
            .optimize = .ReleaseSafe, // keep safety checks for fuzzing
        }),
    });

    b.installArtifact(fuzz_exe);
}
