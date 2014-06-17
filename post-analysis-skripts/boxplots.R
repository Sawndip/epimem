data = read.table("NORMAL")
data00 = read.table("ERGRAPHS",header=TRUE,row.names=1)
data01 = read.table("ERGRAPHS_WLF_0.1",header=TRUE,row.names=1)
data05 = read.table("ERGRAPHS_WLF_0.5",header=TRUE,row.names=1)


data00[,2] = data00[,2]/2
data01[,2] = data01[,2]/2
data05[,2] = data05[,2]/2
#data00 = data00[data00[,5] == 10,]
#data01 = data01[data01[,5] == 10,]
#data05 = data05[data05[,5] == 10,]


boxes = sort(unique(c(9,data00[,2],data01[,2],data05[,2])))
indexer = cbind.data.frame(boxes,1:length(boxes))
colnames(indexer) = c("av.degree","at")

full=cbind.data.frame(data[,3], rep(9,length(data[,3])), rep(indexer[indexer[,1]==9,2],length(data[,3])))
colnames(full) = c("restore.rate","av.degree","at")

box00 = cbind.data.frame(data00[,12],data00[,2],data00[,2])
for(i in 1:dim(box00)[1]){
  box00[i,3] = indexer[indexer[,1] == box00[i,2],2]
}
colnames(box00) = c("restore.rate","av.degree","at")

box01 = cbind.data.frame(data01[,14],data01[,2],data01[,2])
for(i in 1:dim(box01)[1]){
  box01[i,3] = indexer[indexer[,1] == box01[i,2],2]
}
colnames(box01) = c("restore.rate","av.degree","at")

box05 = cbind.data.frame(data05[,14],data05[,2],data05[,2])
for(i in 1:dim(box05)[1]){
  box05[i,3] = indexer[indexer[,1] == box05[i,2],2]
}
colnames(box05) = c("restore.rate","av.degree","at")

pdf("average_degree_drosophilamode.pdf",paper="a4r",width=11,height=7)
postscript("average_degree_drosophilamode.eps",paper="a4",width=11,height=7)
ats = sort(unique(full[,3]))

boxplot(formula = restore.rate ~ at, data = full, boxwex = 0.3, at = ats - 0.375,col="forestgreen",xlab="average degree",ylab="restore rate", ylim=c(0,1.3),xlim=c(1,length(boxes)), main="distribution of restore rate - reset model",axes=FALSE)

ats = sort(unique(box00[,3]))

boxplot(formula = restore.rate ~ at, data = box00, boxwex = 0.15, at = ats - 0.122,col="gold", add=TRUE,axes=FALSE)


boxplot(formula = restore.rate ~ at, data = box00, boxwex = 0.2, at = ats - 0.25,col="gold", xlab="number of elements in largest connected component",ylab="restore rate", ylim=c(0,1.3),xlim=c(1,length(boxes)+0.2), main="distribution of restore rate",axes=FALSE)



ats = sort(unique(box01[,3]))

boxplot(formula = restore.rate ~ at, data = box01, boxwex = 0.15, at = ats + 0.125,col="lightblue", add=TRUE,axes=FALSE)

boxplot(formula = restore.rate ~ at, data = box01, boxwex = 0.2, at = ats ,col="lightblue", add=TRUE,axes=FALSE)

ats = sort(unique(box05[,3]))

boxplot(formula = restore.rate ~ at, data = box05, boxwex = 0.15, at = ats + 0.375,col="deeppink", add=TRUE,axes=FALSE)

boxplot(formula = restore.rate ~ at, data = box05, boxwex = 0.2, at = ats + 0.25,col="deeppink", add=TRUE,axes=FALSE)

axis(2)
axis(1,at=1:length(indexer[,1]),labels=indexer[,1])

legend("topleft",
    legend = c( "Erdoes-Renyi-Graph", "Erdoes-Renyi-Graph, local field 0.1 " , "Erdoes-Renyi-Graph, local field 0.5"),
    fill   = c( "gold" , "lightblue" , "deeppink") )
dev.off()
