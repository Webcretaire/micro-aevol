library(ggplot2)
library(tidyr)
library(dplyr)
library(rstudioapi)
library(tikzDevice)
library(scales)

# Set working directory to script location
setwd(dirname(getActiveDocumentContext()$path))

source("util.R")
source("exports.R")

data <- read.csv("data.csv")

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
  ggplot(gathered_data,
         aes(x = numThreads,
             y = time,
             fill = algorithm)) +
  facet_wrap(gridSize ~ mutationRate) +
  scale_x_continuous(trans = 'log2') +
  scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
  geom_bar(stat = "identity",
           position = position_dodge(),
           colour = "black") +
  theme(legend.position = "bottom") +
  guides(fill = guide_legend(nrow = 4))

export.all("fort", plot)

filtered_data <- subset(data, seqMeasureOneFor != -1)

gathered_data <-
  gather(filtered_data,
         algorithm,
         time,
         starts_with('seqMeasure'))

plot <-
  ggplot(gathered_data,
         aes(x = genomeSize,
             y = time,
             fill = algorithm)) +
  facet_wrap(gridSize ~ mutationRate) +
  scale_x_log10() +
  scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
  geom_bar(stat = "identity",
           position = position_dodge(),
           colour = "black") +
  theme(legend.position = "bottom") +
  guides(fill = guide_legend(nrow = 3))

export.all("seq", plot)

filtered_data <- subset(data, genomeSize == 1000 * numThreads)

gathered_data <-
  gather(filtered_data,
         algorithm,
         time,
         starts_with('ompMeasure'))

plot <-
  ggplot(gathered_data,
         aes(x = numThreads,
             y = time,
             fill = algorithm)) +
  facet_wrap(gridSize ~ mutationRate) +
  scale_x_continuous(trans = 'log2') +
  scale_y_log10(labels = trans_format("log10", math_format(10 ^ .x))) +
  geom_bar(stat = "identity",
           position = position_dodge(),
           colour = "black") +
  theme(legend.position = "bottom") +
  guides(fill = guide_legend(nrow = 4))

print(plot)

export.all("faible", plot)
