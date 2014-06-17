Arg <- commandArgs(trailingOnly = TRUE)
library(igraph)
print(length(Arg))
outfile = Arg[1];
Arg = Arg[-1]

vals = matrix(nrow=length(Arg),ncol=12)
row.names(vals)=gsub(".graph","",Arg)
colnames(vals) = c("alg.connectivity","av.degree","is.connected","conn.components","max.comp.size","number.of.anticorrelations","number.of.correlations","fraction.of.anticorrelations","fractions.of.correlations","possible.patterns","better.patterns","mean.geodistic.distance")
for(i in 1:length(Arg)){
#count anticorrelations

  graphfile = Arg[i];
  pattern=as.numeric(gsub("_[0-9]+","",row.names(vals)[i]))
  probfile = paste(row.names(vals)[i],"probs",sep=".")
  rempat = pattern
  
  vec = numeric(10)
  for(k in 1:10){
    vec[k] = pattern %% 2
    pattern = as.integer(pattern / 2)
  }
  
  print(vec)
  antis = 0
  nonantis = 0
  for(k in 1:9){
    for(j in (k+1):10){
      print(paste(k,j,vec[k],vec[j],sep=" "))
      if(vec[k] == vec[j]){nonantis = nonantis + 1}
      else{antis = antis + 1}
    }
  }
  print(i)
  graph = read.table(graphfile)
  graph = abs(graph)
  row.names(graph) = seq(0,9,1)
  colnames(graph) = seq(0,9,1)
  igraph = graph.adjacency(graph)
  meangeodist = average.path.length(igraph)
  algcon = eigen(graph.laplacian(igraph))$values[2]
  avdeg = mean(degree(igraph))
  conn = is.connected(igraph)
  comp = no.clusters(igraph)
  maxsize = max(clusters(igraph)$csize)
  graph = read.table(graphfile)
  row.names(graph) = seq(0,9,1)
  colnames(graph) = seq(0,9,1)
  igraph = graph.adjacency(as.matrix(graph),weighted=T,mode="upper")
  anti = length(E(igraph)$weight[E(igraph)$weight<0])
  nonanti = length(E(igraph)$weight[E(igraph)$weight>0])
  vals[i,1] = algcon
  vals[i,2] = avdeg
  vals[i,3] = conn
  vals[i,4] = comp
  vals[i,5] = maxsize
  vals[i,6] = anti
  vals[i,7] = nonanti
  if(antis > 0){vals[i,8] = anti / antis }
  else{vals[i,8] = 1}
  if(nonantis > 0){vals[i,9] = nonanti / nonantis }
  else{vals[i,9] = 1}
  probs=read.table(probfile)
  vals[i,10] = length(probs[probs[,2]>0,1])
  patternprob = probs[probs[,1] == rempat,2]
  vals[i,11] = length(probs[probs[,2]>patternprob,2])
  vals[i,12]=meangeodist
}
write.table(vals,file=outfile,quote=FALSE)
