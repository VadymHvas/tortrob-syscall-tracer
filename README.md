# Tortrob Linux x86_64 syscall tracer
Simple syscall tracer based on ptrace.

## Features
- Creating and attaching at existing process.
- Array, structs, enums, flags, masks formatting.
- 90% coverage filesystem related syscalls (others outputs in raw format).

## Build & Run
Clone the repository
```bash
git clone https://github.com/VadymHvas/tortrob-syscall-tracer.git

cd tortrob-syscall-tracer
```

compile it
```bash
make
```

run
```bash
./ttrace <prog>
```

or attach to process
```bash
./ttrace -p <PID>
```

clean
```bash
make clean
```

---
Happy hacking🐂!