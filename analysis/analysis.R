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
source("plots.R")

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
plot <- plot.parallel(filtered_data)
export.all("fort", plot)

filtered_data <- subset(data, seqMeasureOneFor != -1)
plot <- plot.sequential(filtered_data)
export.all("seq", plot)

filtered_data <- subset(data, genomeSize == 1000 * numThreads)
plot <- plot.parallel(filtered_data)
export.all("faible", plot)

filtered_data <- subset(data, genomeSize == 4000 & gridSize == 32 & mutationRate > 5e-06 & mutationRate < 5e-05)
plot <- plot.mixed(filtered_data)
export.all("comp_original", plot)

print(plot)
