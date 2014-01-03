//File: bbcp_Node.C
//
void bbcp_Stor2Buff(void *inFile)
{

    bbcp_File *file = (bbcp_FileSpec *)inFile;
    int retc;

   if (bbcp_Config.Options & bbcp_COMPRESS) retc=file->Read_All(bbcp_APool,1);
      else retc = file->Read_All(bbcp_BPool, bbcp_Config.Bfact);
   DEBUG("File read ended; rc=" <<retc);

    return (void *)retc;
}


//in function bbcp_Node::SendFile()
//Start multiple threads for storage I/O

pthread_t stor_tid[BBCP_MAXSTREAMS+1];
ioSize=bbcp_BPool.DataSize();
stor_num = 4; task_num;
long long offset = 0;


//split the storage I/O task evenly to storage threads
  //open file
   if (!(inFile = fp->FSys()->Open(fp->pathname,O_RDONLY,Mode,fp->fileargs)))
      {bbcp_Emsg("SendFile", errno, Act, fp->pathname);
       exit(2);
      }

  //get the file size

   if((tasksize=inFile->FSp->getSize(inFile->IOB->FD())) < 0)
      {bbcp_Emsg("SendFile", static_cast<int>(-tasksize), "stat", inFile->iofn);
       bbcp_BPool.Abort(); return 200;
      }

  //Close the file
   delete inFile;
  //using the buffer size and file size to compute the offset for each thread,save the result to the matrix
   tasksize -= fp->targetsz;
   if((task_num = tasksize/ioSize) == 0) single_thread_read();
   else stor_num = (stor_num < task_num ? task_num : stor_num);

   offset = fp->targetsz;

for(i=0;i<stor_num;i++)
  {
//Open file and get a bbcp_File object

   if (!(inFile = fp->FSys()->Open(fp->pathname,O_RDONLY,Mode,fp->fileargs)))
      {bbcp_Emsg("SendFile", errno, Act, fp->pathname);
       exit(2);
      }

// If compression is wanted, set up the compression objects
   if (bbcp_Config.Options & bbcp_COMPRESS
       && !(cxp = setup_CX(1, inFile->ioFD()))) return -ECANCELED;


//set the working offset for current thread, special handling for the last thread

  if (offset && ((retc = inFile->Seek(offset)) < 0))
      return bbcp_Emsg("SendFile",retc,"setting read offset for",fp->pathname);

  if(i == stor_num-1)
      inFile->bytesLeft = tasksize + fp->targetsz - offset; 
  else
      inFile->bytesLeft =((task_num/4)+1) * ioSize; 


//create thread and save the tid
if ((retc = bbcp_Thread_Start(bbcp_Stor2Buff,
                                (void *)inFile, &tid))<0)
           {bbcp_Emsg("SendFile",retc,"starting storage thread for",fp->pathname);
            _exit(100);
           }
            stor_tid[i] = tid;
        if (i >= stor_num) {DEBUG("Thread " <<tid <<" assigned to storage data clocker");}
           else {DEBUG("Thread " <<tid <<" assigned to storage stream " <<i);}
    offset += inFile->bytesLeft;
   }


//make sure each thread has terminated normally



//File bbcp_FileSpen.C


}

