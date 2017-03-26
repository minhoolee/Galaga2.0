cc_binary(
  name = "galaga-main",
  deps = [
    "//src:galaga-lib",
  ],
  linkopts = [
    "-L/usr/local/lib",
    "-lsfml-graphics",
    "-lsfml-window",
    "-lsfml-system",
    "-lsfml-network",
    "-lsfml-audio",
    "-lpthread",
  ],
  data = glob([
    "Textures/*.png",
    "Fonts/*.ttf"
  ]),
  srcs = ["main.cpp"],
)
