;; This buffer is for notes you don't want to save, and for Lisp evaluation.
;; If you want to create a file, visit that file with C-x C-f,
;; then enter the text in that file's own buffer.

par(mfrow=c(2,3))

plot(data1[,1],data1[,2],col=rgb(0,18/255,255/255),xlim=c(0,1000),ylim=c(0,100),xlab="generations",ylab="number of cells", col.main = rgb(178/255,149/255,9/255),main = "init: 000000000011111",type="l")
lines(data1[,1],data1[, 4],col=rgb(65/255,178/255,9/255))
lines(data1[,1],data1[, 6],col=rgb(255/255,94/255,25/255))
lines(data1[,1],data1[, 9],col=rgb(0,255/255,245/255))
lines(data1[,1],data1[,10],col=rgb(255/255,4/255,206/255))
lines(data1[,1],data1[,12],col=rgb(178/255,149/255,9/255))


plot(data2[,1],data2[, 2],col=rgb(0,18/255,255/255),xlim=c(0,1000),ylim=c(0,100),xlab="generations",ylab="number of cells", col.main=rgb(255/255,94/255,25/255),main = "init: 000001111100000",type="l")
lines(data2[,1],data2[, 4],col=rgb(65/255,178/255,9/255))
lines(data2[,1],data2[, 6],col=rgb(255/255,94/255,25/255))
lines(data2[,1],data2[, 8],col=rgb(0,255/255,245/255))
lines(data2[,1],data2[,10],col=rgb(255/255,4/255,206/255))
lines(data2[,1],data2[,12],col=rgb(178/255,149/255,9/255))


plot(data3[,1],data3[, 2],col=rgb(0,18/255,255/255),xlim=c(0,1000),ylim=c(0,100),xlab="generations",ylab="number of cells", col.main=rgb(0,18/255,255/255),main = "init: 000001111111111",type="l")
lines(data3[,1],data3[, 4],col=rgb(65/255,178/255,9/255))
lines(data3[,1],data3[, 6],col=rgb(255/255,94/255,25/255))
lines(data3[,1],data3[, 8],col=rgb(0,255/255,245/255))
lines(data3[,1],data3[,10],col=rgb(255/255,4/255,206/255))
lines(data3[,1],data3[,12],col=rgb(178/255,149/255,9/255))


plot(data4[,1],data4[, 2],col=rgb(0,18/255,255/255),xlim=c(0,1000),ylim=c(0,100),xlab="generations",ylab="number of cells", col.main=rgb(255/255,4/255,206/255),main = "init: 111110000000000",type="l")
lines(data4[,1],data4[, 4],col=rgb(65/255,178/255,9/255))
lines(data4[,1],data4[, 6],col=rgb(255/255,94/255,25/255))
lines(data4[,1],data4[, 8],col=rgb(0,255/255,245/255))
lines(data4[,1],data4[,10],col=rgb(255/255,4/255,206/255))
lines(data4[,1],data4[,12],col=rgb(178/255,149/255,9/255))

plot(data5[,1],data5[, 2],col=rgb(0,18/255,255/255),xlim=c(0,1000),ylim=c(0,100),xlab="generations",ylab="number of cells", col.main=rgb(0,255/255,245/255),main = "init: 111110000011111",type="l")
lines(data5[,1],data5[, 4],col=rgb(65/255,178/255,9/255))
lines(data5[,1],data5[, 6],col=rgb(255/255,94/255,25/255))
lines(data5[,1],data5[, 8],col=rgb(0,255/255,245/255))
lines(data5[,1],data5[,10],col=rgb(255/255,4/255,206/255))
lines(data5[,1],data5[,12],col=rgb(178/255,149/255,9/255))
legend("top",legend=c("000001111111111","111111111100000","000001111100000","111110000011111","111110000000000","000000000011111"),col=c(rgb(0,18/255,255/255),rgb(65/255,178/255,9/255),rgb(255/255,94/255,25/255),rgb(0,255/255,245/255),rgb(255/255,4/255,206/255),rgb(178/255,149/255,9/255)),lty=1)

plot(data6[,1],data6[, 2],col=rgb(0,18/255,255/255),xlim=c(0,1000),ylim=c(0,100),xlab="generations",ylab="number of cells", col.main=rgb(65/255,178/255,9/255),main = "init: 111111111100000",type="l")
lines(data6[,1],data6[, 4],col=rgb(65/255,178/255,9/255))
lines(data6[,1],data6[, 6],col=rgb(25/2555,94/255,25/255))
lines(data6[,1],data6[, 8],col=rgb(0,255/255,245/255))
lines(data6[,1],data6[,10],col=rgb(255/255,4/255,206/255))
lines(data6[,1],data6[,12],col=rgb(178/255,149/255,9/255))

