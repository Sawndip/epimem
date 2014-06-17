#args <- commandArgs(trailingOnly = TRUE)
#file = args[1];
file="davidsidea5_57.basinscd"
basindata = read.table(file)
basins = unique(basindata[,1])

#for(i in 1:length(basins)){
i = 2
  bas = basindata[basindata[,1]==basins[i],]
  restores = numeric(100000)
  for(j in 1:100000){
    samp = bas[sample(dim(bas)[1],1000),]
    res = length(samp[samp[,1]==samp[,3],1])/dim(samp)[1];
    restores[j] = res
  }
#}
