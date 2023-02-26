#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->readingThreadCount=0;
  rw->writingThreadCount=0;

  pthread_cond_init(&rw->waitingForWriting, NULL);
  pthread_cond_init(&rw->waitingForReading, NULL);
  
  pthread_mutex_init(&rw->lock, NULL);
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  while (rw->writingThreadCount!=0)
  {
    pthread_cond_wait(&rw->waitingForReading, &rw->lock);
  }
  rw->readingThreadCount++;
  
  pthread_mutex_unlock(&rw->lock);

}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  
  rw->readingThreadCount--;
  pthread_cond_broadcast(&rw->waitingForWriting);
  pthread_mutex_unlock(&rw->lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
  while (rw->writingThreadCount>0 || rw->readingThreadCount >0)
  {
    pthread_cond_wait(&rw->waitingForWriting, &rw->lock);
  }
  rw->writingThreadCount++;
  
  pthread_mutex_unlock(&rw->lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
  
  rw->writingThreadCount--;
  pthread_cond_broadcast(&rw->waitingForWriting);
  pthread_cond_broadcast(&rw->waitingForReading);
  
  pthread_mutex_unlock(&rw->lock);

}
