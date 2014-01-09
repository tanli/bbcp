//File: bbcp_Node.C
//
void *bbcp_Buff2Stor(void *pp)
{

// set up numa affinity
// //
//
    if (bbcp_Config.NumaSpec_SnkFile)
       {
       struct bitmask *numa_snk_file_mask = numa_parse_nodestring(bbcp_Config.NumaSpec_SnkFile);
       if (numa_snk_file_mask == NULL)
          bbcp_Emsg("NUMA", -E2BIG, "parse NumaSpec_SnkFile fail", bbcp_Config.NumaSpec_SnkFile);
       numa_run_on_node_mask(numa_snk_file_mask);
       }

     bbcp_File *fp = (bbcp_File *)pp;
     long retc = fp->Write_All(bbcp_BPool, bbcp_Config.Streams);
     return (void *)retc;
}



//in function bbcp_Node::SendFile()
//Start multiple threads for storage I/O

bbcp_File *outFile;
bbcp_ZCX *cxp[BBCP_MAXSTREAMS];
int stor_num = 4; 


// If we have no IOB, then do a simple in-line passthru


 if (!outFile->IOB) outFile->Passthru(&bbcp_BPool, &bbcp_CPool, 0,  bbcp_Config.Streams);
 else



if (bbcp_Config.Options & bbcp_COMPRESS)
   seqFile = new bbcp_File(Path, 0, 0);

for(i=0;i<stor_num;i++)
  {
   if (bbcp_Config.Options & bbcp_COMPRESS)
     retc = bbcp_Thread_Start(bbcp_Buff2Stor,(void *)seqFile, &tid)
   else
     retc = bbcp_Thread_Start(bbcp_Buff2Stor,(void *)outFile, &tid)
   if(retc < 0)
   {bbcp_Emsg("RecvFile",retc,"starting storage thread for",fp->pathname);
           _exit(100);
   }
   link_tid[dlcount++] = tid;
   if (i >= stor_num) {DEBUG("Thread " <<tid <<" assigned to storage data clocker");}
   else {DEBUG("Thread " <<tid <<" assigned to storage stream " <<i);}
  }

   if (bbcp_Config.Options & bbcp_COMPRESS)
   {if (bbcp_Config.csOpts & bbcp_csPrint && *bbcp_Config.csString)
      cout <<"200 cks: " <<bbcp_Config.csString <<' ' << seqFile->iofn <<endl;

    if (!retc && seqFile->IOB && (bbcp_Config.Options & bbcp_FSYNC)
    && (retc = seqFile->FSp->Fsync((bbcp_Config.Options & bbcp_DSYNC ? seqFile->iofn:0), seqFile->IOB->FD())))
      bbcp_Emsg("Write", -rc, "synchronizing", seqFile->iofn);

    if (seqFile->IOB && (ec = seqFile->IOB->Close()))
      if (!retc) {bbcp_Emsg("Write", -ec, "closing", seqFile->iofn); retc = ec;}
    retc = outFile->Write_All(bbcp_APool, 1);
   }

    if (bbcp_Config.csOpts & bbcp_csPrint && *bbcp_Config.csString)
      cout <<"200 cks: " <<bbcp_Config.csString <<' ' << outFile->iofn <<endl;

    if (!retc && outFile->IOB && (bbcp_Config.Options & bbcp_FSYNC)
    && (retc = outFile->FSp->Fsync((bbcp_Config.Options & bbcp_DSYNC ? outFile->iofn:0), outFile->IOB->FD())))
      bbcp_Emsg("Write", -retc, "synchronizing", outFile->iofn);

    if (outFile->IOB && (ec = outFile->IOB->Close()))
    if (!retc) {bbcp_Emsg("Write", -ec, "closing", outFile->iofn); retc = ec;}


 
//make sure each thread has terminated normally



//File bbcp_FileSpen.C


}

