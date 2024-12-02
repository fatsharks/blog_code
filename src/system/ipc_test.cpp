#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>

void pipe_test()
{
    int fd[2];  // 管道读端是fd[0] 管道写端是fd[1]
    const char str[1024] = "hello world pipe\0";
    char buf[1024];

    if(pipe(fd) < 0)
    {
        std::cout << "pipe create fail" << std::endl;
        return;
    }

    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        std::cout << "fork fail" << std::endl;
        return;
    }
    else if(pid > 0)
    {
        close(fd[0]);

        usleep(1000);

        write(fd[1], str, strlen(str));

        wait(NULL);
    }
    else
    {
        close(fd[1]);

        int len = read(fd[0], buf, sizeof(buf));

        std::cout << "pipe buf: " << buf << std::endl;
    }

}

#define ALL_RW 0666

void fifo_test()
{
    pid_t pid =fork();

    if(pid < 0)
    {
        perror("fork fail");
        return;
    }
    else if(pid > 0)
    {
        const char *fifo_path = "./fifo.txt";
        int ret = mkfifo(fifo_path, ALL_RW);
        if(ret != 0 && errno != EEXIST)
        {
            perror("father create fifo error");
        }

        const char *str = "hello fifo\0";

        int fd = open(fifo_path, O_WRONLY);
        if(fd < 0)
        {
            perror("father open fifo error");
        }

        write(fd, str, strlen(str));
        close(fd);
        return;
    }
    else
    {
        // usleep(500);
        const char *fifo_path = "./fifo.txt";
        int ret = mkfifo(fifo_path, ALL_RW);
        if(ret != 0 && errno != EEXIST)
        {
            perror("son fifo error");
        }

        int fd = open(fifo_path, O_RDONLY);

        if(fd < 0)
        {
            perror("son open fifo error");
        }

        char buf[1024];
        read(fd, buf, sizeof(buf));

        close(fd);

        std::cout << "son read fifo: " << buf << std::endl;
        return;
    }
}

void posix_shm_test()
{
    const char *shm_name = "posix_shm";
    int file_size = 0x400000;
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("main fork fail");
    }
    else if(pid > 0)
    {
        int fd = shm_open(shm_name, O_CREAT | O_RDWR, ALL_RW);

        ftruncate(fd, file_size);
        pthread_rwlock_t rw_lock;
        pthread_rwlockattr_t rw_lock_attr;
        pthread_rwlockattr_init(&rw_lock_attr);
        pthread_rwlock_init(&rw_lock, &rw_lock_attr);

        pthread_rwlockattr_setpshared(&rw_lock_attr, PTHREAD_PROCESS_SHARED);

        pthread_rwlock_wrlock(&rw_lock);

        char *p = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        strcpy(p, "hello posix_shm");

        pthread_rwlock_unlock(&rw_lock);
    
        munmap(p, file_size);

        shm_unlink(shm_name);
    }
    else
    {
        int fd = shm_open(shm_name, O_RDONLY, ALL_RW);

        ftruncate(fd, file_size);

        char *p = (char *)mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);

        std::cout << "posix shm: " << p << std::endl;

        munmap(p, file_size);

        shm_unlink(shm_name);
    }

    return;
}



int ipc_test()
{
    // pipe_test();
    fifo_test();
    // posix_shm_test();

    return 0;
}