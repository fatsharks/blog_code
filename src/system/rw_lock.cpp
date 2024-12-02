#include <shared_mutex>

class rw_lock
{
private:
    std::mutex wr_lock;
    std::shared_lock<std::mutex> rd_lock;
    int rd_cnt;

public:
    rw_lock() : rd_cnt(0) {};
    ~rw_lock();

    void read_lock();
    void read_unlock();
    void write_lock();
    void write_unlock();
};


void rw_lock::read_lock()
{
    rd_lock.lock();
    if(++rd_cnt == 1)
    {
        wr_lock.lock();
    }
    rd_lock.unlock();
}

void rw_lock::read_unlock()
{
    rd_lock.lock();
    if(--rd_cnt == 0)
    {
        wr_lock.unlock();
    }
    rd_lock.unlock();
}

void rw_lock::write_lock()
{
    wr_lock.lock();
}

void rw_lock::write_unlock()
{
    wr_lock.unlock();
}

#include <condition_variable>

class share_mutex
{
private:
    std::mutex mt;
    std::condition_variable rd_cv;
    std::condition_variable wr_cv;

    int rd_active;
    int wr_active;
    int rd_wait;
    int wr_wait;

public:
    share_mutex() : rd_active(0), wr_active(0), rd_wait(0), wr_wait(0) {};
    ~share_mutex();
    void lock_shared();
    bool try_lock_shared();
    void unlock_shared();
    void lock();
    void unlock();
    bool try_lock();
};

void share_mutex::lock_shared()
{
    std::unique_lock<std::mutex> lock(mt);
    if(wr_active)
    {
        rd_wait++;
        while(wr_active)
        {
            rd_cv.wait(lock);
        }
        rd_wait--;
    }
    rd_active++;
}

bool share_mutex::try_lock_shared()
{
    std::unique_lock<std::mutex> lock(mt);
    if(wr_active)
    {
        return false;
    }
    else
    {
        rd_active++;
        return true;
    }
}

void share_mutex::unlock_shared()
{
    std::unique_lock<std::mutex> lock(mt);
    rd_active--;
    if(wr_active == 0 && wr_wait > 0)
    {
        wr_cv.notify_one();
    }
}

void share_mutex::lock()
{
    std::unique_lock<std::mutex> lock(mt);
    if(wr_active || rd_active)
    {
        wr_wait++;
        while(wr_active || rd_active)
        {
            wr_cv.wait(lock);
            wr_active++;
        }
        wr_wait--;
    }
    wr_active++;
}

bool share_mutex::try_lock()
{
    std::unique_lock<std::mutex> lock(mt);
    if(wr_active || rd_active)
    {
        return false;
    }
    else
    {
        wr_active++;
        return true;
    }
}

void share_mutex::unlock()
{
    std::unique_lock<std::mutex> lock(mt);
    wr_active--;
    if(wr_wait)
    {
        wr_cv.notify_one();
    }
    else
    {
        rd_cv.notify_all();
    }
}








