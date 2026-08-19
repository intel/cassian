# logging

## Overview

Stream-style logger with severity levels and a global threshold. Everything lives in
`namespace cassian::logging`.

Messages at or below the current threshold are written; anything more verbose is
discarded cheaply. `fatal` and `error` go to `stderr`, everything else to `stdout`.

## Levels

`LogLevel::fatal` < `error` < `warning` < `info` < `debug` < `trace`.
The default threshold is `info`.

## API

Pick a level by calling the matching function, then stream into it:

```cpp
using namespace cassian;

logging::info() << "running " << test_count << " tests\n";
logging::debug() << "build options: " << options << '\n';
logging::error(logging::Prefix::no_prefix) << raw_driver_message;
```

* `trace()`, `debug()`, `info()`, `warning()`, `error()`, `fatal()` — each returns a
  `Logger&` and takes an optional `Prefix` (`with_prefix` by default, which prints the
  level name once per message).
* `set_threshold(LogLevel)` — change the global threshold; usually driven by a
  command-line argument.
* `is_debug()` — true when the threshold is `debug` or more verbose. Use it to guard
  work that is expensive to compute purely for logging.
* `flush(Logger&)` — streamable manipulator: `logging::info() << "..." << logging::flush;`
* `Logger::setf()` — apply an `std::ostream` manipulator to the underlying stream.
