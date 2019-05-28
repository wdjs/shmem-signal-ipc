#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <utility>

#include "FanzaiIPCClient.h"

FanzaiIPClient::FanzaiIPCClient(string clientName, string serviceName,
                                pid_t clientPid, int bufferSize) {
  this->clientName = clientName;
  this->serviceName = serviceName;
  this->clientPid = clientPid;
  this->bufferSize = bufferSize;

  if (FanzaiIPC.insertProcessToMap(serviceName, pid,
                                   CLIENT_MAP_FILE_LOCATION) == -1) {
    throw "Same service name error\n";
  }

  this->servicePid =
      FanzaiIPC.getPidByName(serviceName, SERVICE_MAP_FILE_LOCATION);

  this->shmemFd = FanzaiIPC.createShmemFd(clientName, bufferSize);
  this->shmemBuf = FanzaiIPC.createShmemBuf(bufferSize, this->shmemFd);
}

int FanzaiIPCClient::sendMessage(IPCMetadata* metadata,
                                 ClientSignalHandler handler) {
  // TODO: add callback
  union sigval sv;
  sv.sival_ptr = (void*)metadata;
  sigqueue(this->servicePid, FANZAI_SIGNAL, sv);
}