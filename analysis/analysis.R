library(ggplot2)
library(tidyr)
library(dplyr)
library(rstudioapi)
library(tikzDevice)
library(scales)

export.png <-
  function(out_name,
           graph,
           width = 1920,
           height = 1080) {
    png(out_name, width, height, units = "px")
    print(graph)
    dev.off()
  }

export.tikz <-
  function(out_name,
           graph,
           width = 19 * 0.393701,
           # Conversion pouces / centimètres
           height = 16 * 0.393701) {
    tikz(file = out_name, width, height)
    print(graph)
    dev.off()
  }

# Set working directory to script location
setwd(dirname(getActiveDocumentContext()$path))

data <- read.csv("out.csv")

names(data) <- c(
  "gridSize",
  "genomeSize",
  "mutationRate",
  "numThreads",
  "ompMeasureMultipleParallelFor",
  "ompMeasureOneParallelFor",
  "ompMeasureOneParallelForBitset",
  "ompMeasureOneParallelSectionMultipleFor",
  "seqMeasureMultipleFor",
  "seqMeasureOneFor",
  "seqMeasureOneForBitset"
)

g_size <- 32000

filtered_data <- subset(data, genomeSize == g_size)

gathered_data <-
  gather(filtered_data,
         algorithm,
         time,
         starts_with('ompMeasure'))

plot <-
  ggplot(gathered_data, aes(x = numThreads, y = time, colour = algorithm)) +
  facet_wrap(gridSize ~ mutationRate) +
  scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
  geom_line() +
  geom_point() +
  theme(legend.position = "bottom") + guides(col = guide_legend(nrow = 4))

export.tikz("omp.tex", plot)

filtered_data <- subset(data, seqMeasureOneFor != -1)

gathered_data <-
  gather(filtered_data,
         algorithm,
         time,
         starts_with('seqMeasure'))

plot <-
  ggplot(gathered_data, aes(x = genomeSize, y = time, colour = algorithm)) +
  facet_wrap(gridSize ~ mutationRate) +
  scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
  geom_line() +
  geom_point() +
  theme(legend.position = "bottom") + guides(col = guide_legend(nrow = 3))

print(plot)

export.tikz("seq.tex", plot)
