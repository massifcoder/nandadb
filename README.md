How to Build NoSQL+ (hybrid) Database in C++ ( Inspired From MongoDb )
Vishal Sharma

Vishal Sharma
6 min read
·
Feb 14, 2024

NoSql databases are amazing because of their performance for read and write.

Since we are taking inspiration from MongoDb, so firstly I want to give hats off to the developers of the MongoDb for their amazing work.

To build NoSQL database for project, we will take inspiration from MongoDb and would like to replicate at some places.

Before designing the architecture of the database (project) we need to define the output i.e. what we want and what is expected.

Ouput/Expectations:

    Able to run queries on console and provide api’s to run command from third application or network api.
    Able to build different databases, documents inside databases.
    Able to do CRUD operations ( create, read, update and delete ).
    POST ( Power on self testing to handle faulty behaviour ).
    Instant fault handling. ( To save work if suddenly system crash and handle fault. )

In this article we will not fulfill network api part.

Steps:

We will use bottom down approach, firstly handle the crud operations.

1. Architecture:
Architecture of Database Fig-I

Concept to learn before next step:

    Log Structured Merge Tree:

A log-structured merge-tree (LSM tree) is a data structure used for storing and managing key-value pairs, optimized for write-heavy workloads and scenarios where data may not fit entirely in memory. LSM trees are commonly used in database systems and distributed storage systems to provide efficient storage and retrieval of data.

The basic idea behind an LSM tree is to maintain multiple levels of storage, each optimized for different types of operations. It combines the sequential write performance of append-only logs with the efficient random access of traditional tree structures. The key components of an LSM tree typically include:

1. Log: Incoming data is initially written to an append-only log, which is a sequentially written data structure. This log provides fast writes since new data is simply appended to the end.

2. Memtable: Data from the log is periodically flushed into an in-memory data structure called a memtable. The memtable is usually implemented as a sorted data structure, such as a sorted array or a skip list, which allows for efficient reads and writes.

3. Immutable Sorted Files (SSTables): When the memtable reaches a certain size threshold, it is flushed to disk as an immutable sorted file (often referred to as an SS table). These files are sorted by key, which enables efficient range queries and merges.

4. Leveling: LSM trees typically maintain multiple levels of sorted files on disk. These levels are organized such that files at higher levels contain merged data from lower levels, with each level typically containing larger and less frequently accessed files.

5. Merge: Periodically, the sorted files are merged together to reduce the number of files and improve read performance. During the merge process, duplicate keys are resolved, and the resulting files are written to a higher level in the tree.

    Next Step:

To make our database more efficient, we will not use this LSM but will get inspiration from this.

    Whenever user store any new data, it will be written to memotable. Memotable can be B-tree, but to add the property of range query we will use B+ tree.

Figure-II

Our memotable will be this type of B+ tree, in each node of this memotable instead of storing data directly, we will store 2 variables i.e. one is primary key and other is data to be store (different datatype -> Binary Javascript Object Notation), again inspired from mongodb bson datatype.

Whenever user is going to add data, it will be added to B+ tree, since B+ tree store data in unique way that it is height balanced and data remain sorted in inorder traversal. Since, we are using tree concept so searching in this structure will be O(log n base height of tree).

    We will limit the size of the B+ tree or B- tree depend upon your project, max count of the B+ tree will be 2^n-1. So, that tree will be full tree. Once, it crosses our limit we will flush this B+ tree to SST level 1.
    Since its full tree, means there will not be any space between nodes and space will be optimised when we will store it in disk.

Flush To Disk ( SST Level 1 ) :

We can not store this B+ tree directly to disk, and to optimise the reading in disk we will follow such steps:

    At every sst level, we will maintain two files. One file will be indexed file, and other data (SST Lv1) file.
    We will store this data into disk as we get in level wise traversal, so that we will get the feature of the fast read and also parameter ( range query searching ).

Storage structure:

From fig-I, the data will be stored in disk as:

In level1.ind file :

| 19 | -> | 40 | -> | 10 | -> | 15 | -> | 25 | -> | 30 | -> | 51 | -> | 99 | -> | 5 | -> | 10 | -> | 5 | -> | 25 | -> | 30 | -> | 51 | -> | 60 | -> | 99 | -> | 120 |

In data.sst file:

| 5 | -> | 25 | -> | 30 | -> | 51 | -> | 60 | -> | 99 | -> | 120 |

when we were writting this data, then we will store the address of this data node in .ind file with the primary key. So, that we can get the address where this data is, and then we will get the data from that file directly since we have direct address.

    Optimisation: if user is running only the search of one value, then we will not go to leaf side of the .ind file’s tree and directly get value because we get the address. But, if query is in form of range. Then, we will go till leaf and get the range of addresses, where all data is stored.

Flush To SST next levels:

In every SST level we store number of trees in power of 2. Means at SST level 1, we will store 2 memotables. And then if SST level 1 is filled, then we will merge both memotables of level 1 and create a SST1 and then flush SST1 to level 2. Where SST lv2 store 4 SST1.

Even if LV2 is filled, then merge all SST1 into one SST2 and flush to next level, and this step will repeat. And maintain this data in meta data file.

Optimisations:

To increase the read speed we will use bloom filters, means will ignore searching in those levels, where its gurantee that value can not be present.

    Method to Update and Delete:

Till now we have covered that how we will create and read with best optimisations, but to make a database faster we not only need fast read and write, we want fast update and delete.

Instead of going to place and delete at that value and updating by visiting that place, what we will do. We use different approach that we will add new node with updated data and in actual location, we set delete variable true.

So, during search it will get only updated value. And to decrease this unwanted deleted node, we will make sure that whenever that deleted data is going to flush to next level, we will remove that node and space will be saved and our time is saved and we get good optimisation.

Same approach we will use to optimise it.

Next article:

Merge this part with other part which handle network api and failure and acid property function with multithreading concepts.

References:

MongoDB is built on a different storage engine architecture known as WiredTiger. All information for inspiration is gathered from their guide or docs :
WiredTiger: Log-Structured Merge Trees
A common requirement is sustained throughput under a workload that consists of random inserts, where either the key…

source.wiredtiger.com
