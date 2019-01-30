library(ggplot2)
library(tidyr)
library(dplyr)
library(rstudioapi)

export.png <-
  function(out_name,
           graph,
           width = 1920,
           height = 1080) {
    png(out_name, width, height, units = "px")
    print(graph)
    dev.off()
  }

# Set working directory to script location
setwd(dirname(getActiveDocumentContext()$path))

data <- read.csv("out.csv")

g_size <- 32000

filtered_data <- subset(data, genome_size == g_size)

gathered_data <-
  gather(
    filtered_data,
    algorithm,
    time,
    starts_with('omp_singleMeasure')
  )

plot <-
  ggplot(gathered_data, aes(x = num_threads, y = time, colour = algorithm)) +
  facet_wrap(grid_size ~ mutation_rate) +
  scale_y_log10() +
  geom_line() +
  geom_point()

export.png("omp.png", plot)

filtered_data <- subset(data, seq_singleMeasure_oneFor != -1)

gathered_data <-
  gather(
    filtered_data,
    algorithm,
    time,
    starts_with('seq_singleMeasure')
  )

plot <-
  ggplot(gathered_data, aes(x = genome_size, y = time, colour = algorithm)) +
  facet_wrap(grid_size ~ mutation_rate) +
  scale_y_log10() +
  geom_line() +
  geom_point()

export.png("seq.png", plot)

