#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>


#define MAXFNAME	14
#define BLKSIZE		512
#define FS_CLEAN 0
#define FS_SIZE 4194304 //512 * 8192
#define NUMBER_OF_BLOCKS 8192
#define BOOTBLK 1
#define SUPERBLK 2
#define INODEBLK 8
#define ROOTDIRBLK 4



char *devfiles[] = {"TEST",NULL};
int devfd[] = {-1, -1};

// Data structure definitions
struct SupBlock {
	char sb_vname[MAXFNAME]; //super block name
	int	sb_nino; // number of inodes
	int	sb_nblk; // number of blocks
	int	sb_nrootdir; // inode of root dir
	int	sb_nfreeblk;  //no of free blocks
	int	sb_nfreeino; //no of free inodes
	int	sb_flags; //
	unsigned short sb_freeblks[NUMBER_OF_BLOCKS];
	int	sb_freeblkindex; //free block index
	int	sb_freeinoindex;	// free cache index to start searching from
	unsigned int	sb_chktime; //last check time
	unsigned int	sb_ctime; // creation time 
	unsigned short sb_freeinos[85]; //INODETABSIZE basically
};

struct OnDiskDirEntry {
	char	d_name[MAXFNAME]; //directory name 
	unsigned short	d_ino; //inode number of directory 
};

struct DirEntry {
	struct OnDiskDirEntry d_entry; //dir entry
	int	d_offset; //offset while opening using O_APPEND
}; 



struct INode {
	unsigned int	i_size; 	//file size
	unsigned int	i_atime;	//last access time
	unsigned int	i_ctime;	//creation time
	unsigned int	i_mtime;	//last modified time 
	unsigned short	i_blks[13];	//data links 12 direct + 1 direct
	short		i_mode; 		//permissions
	unsigned char	i_uid;		// user id						
	unsigned char	i_gid;		// group id
	unsigned char	i_gen;		
	unsigned char	i_lnk;		//no of symbolic links 
};

#define INODETABSIZE	((INODEBLK * BLKSIZE) / sizeof(struct INode))

struct InCoreINode {
	struct INode 	ic_inode;
	int 		ic_ref;
	int		ic_ino;
	short		ic_dev;
	struct InCoreINode	*ic_next;
	struct InCoreINode	*ic_prev;
};


struct OpenFileObject {
	struct InCoreINode	*ofo_inode;
	int			ofo_mode;
	unsigned int		ofo_curpos;
	int			ofo_ref;
};
#define ROOTDIRSIZE		((4*512)/sizeof(struct OnDiskDirEntry))
//============= TESTING APPLICATION USING THESE FS CALLS ==============

// Menu driven testing application for creation of fs, 
// and all file and directory related operations
int main()
{
	// printf("hello worldS\n");
	OpenDevice(0);
	MkFS(0);
	ShutdownDevice(0);
	// printf("%ld\n", INODETABSIZE);
}



struct SupBlock sb;
char nullbuf[512];
struct INode nullINode;


int reservblks = 1 + 2 + 8 + 4; // boot, super, inodetable, rootdir blks

//============= SYSTEM CALL LEVEL NOT FOLLOWED =======

//============= VNODE/VFS NOT FOLLOWED ===============

//============== UFS INTERFACE LAYER ==========================


int dev;
//function to seekto a particular block in the device(file)


int MkFS(int dev_no)
{
	dev = dev_no;
	// int rootentrycount = ROOTDIRSIZE;
	int inodecount = INODETABSIZE;
	// int blkcount = 8192;
	
	
	

	// Assuming only predefined sizes are used
	// Otherwise we have to recompute according to the parameters given.
	//
	
	// Boot block dummy block (Because no boot loader nothing...)
	bzero(nullbuf, BLKSIZE);
	write(devfd[dev], nullbuf, BLKSIZE);	
	
	// Write initialized superblock
	strcpy(sb.sb_vname, "TESTFILESYS");
	sb.sb_nino = inodecount;
	sb.sb_nblk = NUMBER_OF_BLOCKS;
	sb.sb_nrootdir = 0; //inode entry number of the root directory
	sb.sb_nfreeblk = NUMBER_OF_BLOCKS - reservblks;
	sb.sb_nfreeino = inodecount;
	sb.sb_flags = FS_CLEAN;
	sb.sb_chktime = 0;
	sb.sb_ctime = 0;

	sb.sb_freeinoindex = 0;// don't know what it's for
	sb.sb_freeinoindex = 0;// don't know either
	int i, count;
	for (i = 0; i < NUMBER_OF_BLOCKS; ++i)
	{
		if (i <= reservblks){
			sb.sb_freeblks[i] = -1; // -1 indicating occupied
		}
		else{
			sb.sb_freeblks[i] = 1; // 1 indicating free
		}
	}
	count = 0;
	// initialization of free inodes list
	for (i = 1; i < INODETABSIZE; ++i)
	{
		sb.sb_freeinos[count] = -1; // storing -1 for not free.
		count++;
	}
	writeSuperBlock();
	// write(devfd[dev], &sb, sizeof(struct SupBlock));
	// write(devfd[dev], nullbuf, (SUPERBLK*BLKSIZE - sizeof(struct SupBlock))); // writing null data to the rest of the block space allocated for the super block


	printf("Superblock written\n");



	// Write initialized list of inodes
	nullINode.i_size = 0;
	nullINode.i_atime = 0;
	nullINode.i_ctime = 0;
	nullINode.i_mtime = 0;
	bzero(nullINode.i_blks, 13);
	nullINode.i_mode = 0;
	nullINode.i_uid = 0;
	nullINode.i_gid = 0;
	nullINode.i_gen = 0;
	nullINode.i_lnk = 0;
	for(i=0; i<INODETABSIZE; i++)
		WriteInode(devfd[dev], i, &nullINode);
		// write(devfd[dev], &nullINode, sizeof(struct INode));

	printf("Inodes initialized!\n");
	
	// Write initialized list of directory entries
	


	// Fill the remaining empty datablocks
	
	// Write free blk information (data structures)
}


// Open/create a file in the given directory with specified uid, gid 
// and attributes. Type of modes are limited to read, write and append only.
int OpenFile(int dirhandle, char *fname, int mode, int uid, int gid, int attrib)
{
}

// Close a file
int CloseFile(int fhandle)
{
}

// Read from a file already opened, nbytes into buf
int ReadFile(int fhandle, char buf[], int nbytes)
{
}

// Write into a file already opened, nbytes from buf
int WriteFile(int fhandle, char buf[], int nbytes)
{
}

// Move the file pointer to required position
int SeekFile(int fhandle, int pos, int whence)
{
}

// Create a directory
int MkDir(int pardir, char *dname, int uid, int gid, int attrib)
{
}

// Delete directory (if itis empty)
int RmDir(int pardir, char *dname)
{
}

int OpenDir(int pardir, char *dname)
{
}

int CloseDir(int dirhandle)
{
}

int SeekDir(int dirhandle, int pos, int whence)
{
}

int ReadDir(int dirhandle, struct DirEntry *dent)
{
}
int WriteDir(int dirhandle, struct DirEntry *dent)
{
}

//============== UFS INTERNAL LOW LEVEL ALGORITHMS =============

int ReadInode(int dev, int ino, struct INode *inode)
{
	
	seekTo(3); // inode block starts after 3 blokcs
	seekBytes(ino * sizeof(struct INode)); //seek to the required inode
	read(dev, &inode, sizeof(struct INode));
	inode->i_atime = time(NULL);
	WriteInode(dev, ino, inode);// updating the last accessed time 

}
//write an inode structure to the device(file)
int WriteInode(int dev, int ino, struct INode *inode)
{
	seekTo(3); // inode block starts after 3 blokcs
	seekBytes(ino * sizeof(struct INode)); //seek to the required inode
	inode->i_mtime = time(NULL); // updating the last modified time 
	write(dev, inode, sizeof(struct INode));

}
// find a free inode from the freeinos list of the superblock and return a free inode and update status
int AllocInode(int dev)
{
	int size = INODETABSIZE;
	int index = sb.sb_freeinoindex;
	int count = 0;
	while(index == -1 && count < size){
		index = (index + 1)%size; // finding the next free inode
		count++;
	}
	if (index == -1){
		printf("Inode could not be allocated\n");
		exit(1);
	}
	readSuperBlock();
	sb.sb_freeinos[index] = -1;
	sb.sb_freeinoindex = index;
	writeSuperBlock();


	return index;


}
// free an inode i.e write null inode in the file and update free inode list in superblock
int FreeInode(int dev, int ino)
{
	WriteInode(dev, ino, &nullINode);
	readSuperBlock();
	sb.sb_freeinos[ino] = 1;
	sb.sb_freeinoindex = ino;
	writeSuperBlock();

}

int AllocBlk(int dev)
{
	int size = NUMBER_OF_BLOCKS;
	int index = sb.sb_freeblkindex;
	int count = 0;
	while(index == -1 && count < size){
		index = (index + 1)%size; // finding the next free inode
		count++;
	}
	if (index == -1){
		printf("block could not be allocated\n");
		exit(1);
	}
	readSuperBlock();
	sb.sb_freeblks[index] = -1;
	sb.sb_freeblkindex = index;
	writeSuperBlock();


	return index;
}

int FreeBlk(int dev, int blk)
{
	if(blk <reservblks ){
		printf("Can't free reserved blocks\n");
		exit(1);
	}
	seekTo(blk);
	write(dev, nullbuf, BLKSIZE);
	readSuperBlock();
	sb.sb_freeblks[blk] = 1;
	sb.sb_freeblkindex = blk;
	writeSuperBlock();
}

int readSuperBlock(){
	seekTo(0);
	read(devfd[dev], &sb, sizeof(struct SupBlock));
	return 0;
}

int writeSuperBlock(){
	seekTo(0);
	write(devfd[dev], &sb, sizeof(struct SupBlock));
	write(devfd[dev], nullbuf, (SUPERBLK*BLKSIZE - sizeof(struct SupBlock)));
	return 0;
}

int seekBytes(int bytes){
	lseek(devfd[dev], bytes, SEEK_CUR);
}

int seekTo(int blk){
	lseek(devfd[dev], blk * BLKSIZE, SEEK_SET);
}

//============== DEVICE DRIVER LEVEL =====================

// // Reading a logical block blk from device dev
// int ReadBlock(int dev, int blk, int buf[BLKSIZE])
// {
// 	// Check for validity of the block
// 	// Check for validity of the device

// 	// If OK read the block
// 	lseek(devfd[dev], blk * BLKSIZE, SEEK_SET);
// 	return read(devfd[dev], buf, BLKSIZE);
// }

// // Writing a logical block blk to device dev
// int WriteBlock(int dev, int blk, int buf[BLKSIZE])
// {
// 	// Check for validity of the block
// 	// Check for validity of the device

// 	// If OK write the block
// 	lseek(devfd[dev], blk * BLKSIZE, SEEK_SET);
// 	return write(devfd[dev], buf, BLKSIZE);
// }



// Open the device
int OpenDevice(int dev)
{
	// Open the device related file for both reading and writing.
	//

	if ((devfd[dev] = open(devfiles[dev], O_RDWR|O_CREAT, 0666)) < 0)
	{
		perror("Opening device file failure:");
		exit(0);
	}
	return devfd[dev];
}

// Shutdown the device
int ShutdownDevice(int dev)
{
	// if (dev < ... && dev > ...)
	if (devfd[dev] >= 0)
		close(devfd[dev]);

	return 0;
}

