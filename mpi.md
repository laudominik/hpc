# Simple

```
MPI_Init(NULL, NULL);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &world);
```

# Blocking communication

```
MPI_Recv(
    void *buffer, int count, MPI_Datatype, 
    int source, int tag,
    MPI_Comm comm, MPI_Status *status
)

MPI_Send(
    void *buffer, int count, MPI_Datatype,
    int destination, int tag, 
    MPI_Comm comm
)
MPI_Probe(int source,int tag, MPI_Comm, MPI_Status*) // check if message is ready
MPI_Get_count(MPI_Status *status,MPI_Datatype dtype,int *count)
```

```
MPI_ANY_TAG    // matches any tag
MPI_ANY_SOURCE // matches any source
MPI_COMM_WORLD // matches any comm
```

Optimized sends/receives
```
Ssend(...)     // synchronized
MPI_Rsend(...) // "ready"
```

# Non-blocking communication
```
MPI_Isend(…,MPI_Request *request)
MPI_Irecv(…,MPI_Request *request)

MPI_Iprobe(int source,int tag,MPI_Comm,int *flag,MPI_Status
*status) // like probe but nonblocking
```

# Waiting (blocking)
```
MPI_Wait(MPI_Request *,MPI_Status *)
MPI_Waitany(int count,MPI_Request *,int *index MPI_Status *status)
MPI_Waitall(int count,MPI_Request *,int *index MPI_Status *status)
```

# Testing (non-blocking)
Check for completion
```
MPI_Test(MPI_Request *request,int *flag,MPI_Status *status)
MPI_Test[any|all](int count,MPI_Request *,int *index,int *flag,MPI_Status *)
```

# Status
```
MPI_Status status;
status.MPI_TAG; status.MPI_SOURCE; status.MPI_ERROR;
```

# Datatypes
```
MPI_Type_contiguous(int count,MPI_Datatype, MPI_Datatype *newdatatype)
MPI_Type_vector(int count,int blocklength,int
stride,MPI_datatype,MPI_Datatype *newdatatype) // separate by stride
```

# Collective communication
```
MPI_Barrier(MPI_Comm)
MPI_Gather()
MPI_Scatter()
```

# Other
```
MPI_Cancel(MPI_Request*)
MPI_Comm_spawn
MPI_Comm_spawn_multiple
```