or=read.table("or")
cd=read.table("cd")
m=matrix(rep(0,max(or[,3])*(max(or[,2])+1)),nrow=max(or[,2])+1,ncol=max(or[,3]))
c=matrix(rep(0,max(or[,3])*(max(or[,2])+1)),nrow=max(or[,2])+1,ncol=max(or[,3]))
mcd=matrix(rep(0,max(or[,3])*(max(or[,2])+1)),nrow=max(or[,2])+1,ncol=max(or[,3]))
ccd=matrix(rep(0,max(or[,3])*(max(or[,2])+1)),nrow=max(or[,2])+1,ncol=max(or[,3]))

for(i in 1:dim(or)[1]){
      c[or[i,2]+1,or[i,3]]=c[or[i,2]+1,or[i,3]]+1;
      m[or[i,2]+1,or[i,3]]=m[or[i,2]+1,or[i,3]]+or[i,4];
}
for(i in 1:dim(cd)[1]){
      ccd[cd[i,2]+1,cd[i,3]]=ccd[cd[i,2]+1,cd[i,3]]+1;
      mcd[cd[i,2]+1,cd[i,3]]=mcd[cd[i,2]+1,cd[i,3]]+cd[i,4];
}

for(i in 1:max(cd[,2])+1){
      for(j in 1:max(cd[,3])){
      	    if(c[i,j] >0){m[i,j]=m[i,j]/c[i,j];}
	    else{m[i,j]=NA}
      	    if(ccd[i,j] >0){mcd[i,j]=mcd[i,j]/ccd[i,j];}
	    else{mcd[i,j]=NA}
      }
}

dil=matrix(rep(0,max(or[,3])*(max(or[,2])+1)),nrow=max(or[,2])+1,ncol=max(or[,3]))

for(i in 1:max(cd[,2])+1){
      for(j in 1:max(cd[,3])){
      	    if(is.na(m[i,j]) || is.na(mcd[i,j])){dil[i,j]=NA}
	    else{
		dil[i,j]=m[i,j]-mcd[i,j]
	    }
      }
}

par(mfrow=c(3,1))
colramp = colorRampPalette(c("yellow","orange","red","green","blue"))
image(1:dim(m)[2],0:dim(m)[1]-1,t(m),col=colramp(100),zlim=c(0,1))
image(1:dim(m)[2],0:dim(m)[1]-1,t(mcd),col=colramp(100),zlim=c(0,1))
image(1:dim(m)[2],0:dim(m)[1]-1,t(dil),col=colramp(100),zlim=c(-1,1))
