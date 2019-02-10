plot.sequential <-
  function(filtered_data) {
    gathered_data <-
      gather(filtered_data,
             algorithm,
             time,
             starts_with('seqMeasure'))
    
    return(
      ggplot(gathered_data,
             aes(
               x = genomeSize,
               y = time,
               fill = algorithm
             )) +
        facet_wrap(gridSize ~ mutationRate, labeller = label_both) +
        scale_x_log10() +
        scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
        geom_bar(
          stat = "identity",
          position = position_dodge(),
          colour = "black"
        ) +
        theme(legend.position = "bottom") +
        guides(fill = guide_legend(nrow = 3))
    )
  }

plot.parallel <-
  function(filtered_data) {
    gathered_data <-
      gather(filtered_data,
             algorithm,
             time,
             starts_with('ompMeasure'))
    
    return(
      ggplot(gathered_data,
             aes(
               x = numThreads,
               y = time,
               fill = algorithm
             )) +
        facet_wrap(gridSize ~ mutationRate, labeller = label_both) +
        scale_x_continuous(trans = 'log2') +
        scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
        geom_bar(
          stat = "identity",
          position = position_dodge(),
          colour = "black"
        ) +
        theme(legend.position = "bottom") +
        guides(fill = guide_legend(nrow = 4))
    )
  }

plot.mixed <- 
  function(filtered_data) {
    gathered_data<-
      gather(filtered_data,
             algorithm,
             time,
             contains('Measure'))
    
    return(
      ggplot(gathered_data,
             aes(
               x = numThreads,
               y = time,
               fill = algorithm
             )) +
        scale_x_continuous(trans = 'log2') +
        scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
        geom_bar(
          stat = "identity",
          position = position_dodge(),
          colour = "black"
        ) +
        theme(legend.position = "bottom") +
        guides(fill = guide_legend(nrow = 4)) + 
        geom_hline(yintercept=6097, linetype="dashed", color = "red")
    )
  }