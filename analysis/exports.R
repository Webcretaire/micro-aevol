export.png <-
  function(out_name,
           graph,
           width = 1920,
           height = 1080) {
    png(glue("PNG/", out_name), width, height, units = "px")
    print(graph)
    dev.off()
  }

export.tikz <-
  function(out_name,
           graph,
           width = 19 * 0.393701,
           height = 16 * 0.393701) {
    tikz(file = glue("TEX/", out_name), width, height)
    print(graph)
    dev.off()
  }

export.all <-
  function(out_name,
           graph) {
    export.png(glue(out_name, ".png"), graph)
    export.tikz(glue(out_name, ".tex"), graph)
  }