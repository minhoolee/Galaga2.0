filegroup(
  name = "srcs",
  srcs = glob([
    "src/**/*.cpp"
  ]),
)
cc_library(
  name = "galaga-lib",
  srcs = glob(["**/*.cpp"]),
  hdrs = glob(["**/*.hpp"]),
  visibility = ["//:__pkg__"],
)
