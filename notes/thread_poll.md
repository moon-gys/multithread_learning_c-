## 线程池

<https://www.jianshu.com/p/210eab345423>
<https://www.cnblogs.com/ailumiyana/p/10016965.html>

### 为什么使用线程池

目前的大多数网络服务器，包括Web服务器、Email服务器以及数据库服务器等都具有一个共同点，就是单位时间内必须处理数目巨大的连接请求，但处理时间却相对较短。传统多线程方案中我们采用的服务器模型则是一旦接受到请求之后，即创建一个新的线程，由该线程执行任务。任务执行完毕后，线程退出，这就是是“即时创建，即时销毁”的策略。

尽管与创建进程相比，创建线程的时间已经大大的缩短，但是如果提交给线程的任务是执行时间较短，而且执行次数极其频繁，那么服务器将处于不停的创建线程，销毁线程的状态。

1. 创建/销毁线程伴随的系统开销，频繁的创建/销毁线程影响处理效率。
  ```
  例如：
  记创建线程消耗时间T1，执行任务消耗时间T2，销毁线程消耗时间T3

  如果T1+T3>T2，那么是不是说开启一个线程来执行这个任务太不划算了！

  正好，线程池缓存线程，可用已有的闲置线程来执行新任务，避免了T1+T3带来的系统开销
  ```
2. 线程并发数量过多，抢占系统资源从而导致阻塞
  ```
  线程能共享资源，如果同时执行的线程过多，就有可能导致系统资源不足而产生阻塞的情况。

  运用线程池能有有效的控制线程的最大并发数，避免上述问题。
  ```
  3. 对线程进行一些简单的管理
  ```
  延时执行、定时循环执行的策略
  ```
### 什么是线程池

线程池（thread pool）是一种线程使用模式。线程过多或者频繁创建和销毁线程会带来调度开销，进而影响缓存局部性和整体性能。而线程池维护着多个线程，等待着管理器分配可并发执行的任务。这避免了在处理短时间任务时创建与销毁线程的代价，以及保证了线程的可复用性。线程池不仅能够保证内核的充分利用，还能防止过分调度。

可用线程数量应该取决于可用的并发处理器、处理器内核、内存、网络sockets等的数量。

### 线程池的组成部分

- 线程池管理器（ThreadPool
  - 创建并管理线程池，包括创建、销毁线程池、添加新任务等。
- 工作队列（PoolWorker）
  - 线程池中的线程，在没有任务时处于等待状态，可以循环的执行任务。
- 任务接口（Task）
  - 每个任务必须实现的接口，以供工作线程调度任务的执行，它主要规定了任务的入口，执行任务完成后的首尾工作，任务的执行状态等。
- 任务队列（taskQueue)
  - 用于存放没有处理的任务。提供一种缓冲机制。
  
  线程池的外部支持
  
  - 锁
  - 条件变量

### 线程池工作的四种情况

假设线程池大小为3，任务队列不做限制

- 主程序当前没有任何任务要执行，线程池中的任务队列为空闲状态
- 主程序添加小于等于线程池中线程数量的任务
- 主程序添加大于任务数量大于当前线程池中的线程数量的任务
- 主程序添加任务数量大于当前线程池中线程数量的任务，且任务缓冲队列已满

**1主程序当前没有任何任务要执行，线程池中的任务队列为空闲状态**

<img="images/one.png" height=399 lenght=450>

**2 主程序添加小于等于线程池中线程数量的任务**

<img="images/two.png" height=399 lenght=450>

**3 主程序添加大于任务数量大于当前线程池中的线程数量的任务**

<img="images/three.png" height=399 lenght=450>

**4 主程序添加任务数量大于当前线程池中线程数量的任务，且任务缓冲队列已满**

<img="images/four.png" height=399 lenght=450>

### code

## 回调函数

<https://blog.csdn.net/webzhuce/article/details/84947380>
<https://zhuanlan.zhihu.com/p/83943973>

1. 什么是回调函数，就是以函数指针做参数传递给另一个函数，当这个指针被用来调用其所指向的函数时，就是回调函数。
把一段可执行的代码像参数传递那样传给其他代码，而这段代码会在某个时刻被调用执行，这就叫做回调。如果代码立即被执行就称为同步回调，如果在之后晚点的某个时间再执行，则称之为异步回调。

2. 什么场景下需要使用回调函数
- 回调函数主要是完成通知功能， 例如我领导给我分配另外工作， 但是我手里工作又没做完， 那领导总不可能没过个几个小时就跑过来问一下，这样我们两个人都很烦，而在代码中如果这样实现（需要开一个线程， 不停的轮寻）也很消耗系统资源， 所以最好的办法就是等我手里的工作完成了， 我自己去和领导说，然后进入下个工作任务， 而在我们代码中想要这样实现就需要通过回调函数来完成。

- 编程分为两类：系统编程(system programming)和应用编程(application programming)。
  - 系统编程，通俗地讲就是编写库；
  - 应用编程就是利用写好的各种库去实现自己想要的具体功能，这就是常说的应用开发，系统程序员(编写库的)会给自己的库留一些接口，即应用编程接口API(application programming interface)，这些接口用来给应用程序员传入参数调用库函数时使用。

一般情况下，我们的应用程序会调用已经写好的库函数，实现预先已经写好的库函数功能，但有些库函数却要求应用程序先给它传一个函数，然后才能实现相应的功能，这个被传入的，后又被调用的函数被称为回调函数(callback function)。

3. 回调函数，本质也是一种函数调用，先将函数以指针的方式传入，然后，调用。这种写法的好处是，那就是解耦，对外提供函数类型，而不是函数定义。这样我们只需要依据函数类型和函数功能提供函数就可以了。给程序的书写带来了很大的自由。

4. 回调函数在c++中的四种类型

- 函数指针 
```
#include<stdio.h>

//第一步，声明函数指针。下面是声明一个函数指针，这个函数指针的类型名叫func，它指向的函数返回值为void，调用的参数为int
typedef void (*func)(int);

void Print(int i)
{
    printf("%d\n", i);
}
struct Mystruct
{
    int param;
    func callback;
};

int main()
{
    // //  第二步，定义一个函数指针，并且指向一个符合函数指针声明的函数
    // func f = &Print;
    // f(1);  //调用函数   

    Mystruct m_struct;
    m_struct.param = 1;
    m_struct.callback = &Print;
    m_struct.callback(m_struct.param);

    return 0;
}
```
- 多态
  函数指针无法指向成员函数。（可以把要回调的函数声明为static）。不过既然都用到了类封装,回调又要怎样实现呢？
  - 用多态。可以定义一个只包含若干个纯虚函数的接口类，然后每个具体类都继承这个接口类并且把这个类的指针传给回调者，回调者在回调的时候就能因为多态的特性实现调用对应的实现了.
  ```
  #include <iostream>

  class Interface{
    public:
      virtual void do_something() = 0;
      virtual ~Interface(){}
  };

  class Impl : public Interface {
    public:
    virtual ~Impl(){}
    virtual void do_something(){
       std::cout <<"Impl::dosomething" << std::endl;
    }
  };

  int main(){
    Interface *f = new Impl();
    f->do_something();
    return 0;
  }
  ```
- function 和 bind
  - 1. function的功能和函数指针类似, 可以通过改变指向的对象来实现动态修改函数的行为。
    - 区别在于函数指针只能针对共有的非成员函数，而function则可以用于调用c++中的各种可调用对象。
    - 那什么是可调用对象呢？简单地说就是能够通过()来调用的对象，包括函数，functor还有c++11的lambda表达式。
    
    ```
    /* function去调用普通函数*/
    
    #include <stdio.h>
    #include <functional>

    //第一步，声明函数指针。下面是声明一个函数指针，这个函数指针的类型名叫func，它指向的函数返回值为void，调用的参数为int
    //typedef void(*func)(int);
    typedef std::function< void(int)> func;

    void Print(int i ){
      printf("%d\n", i);
    }

    int main()
    {
        func f = &Print;
        f(1);
        return 0;
    }
    ```
    - function并不能直接调用成员函数的，需要通过两种方式的，第一种比较原始的做法，就是把类变成一个functor(仿函数)，然后在operator()的重载中去调用成员函数。第二种结合bind()
    <cnblogs.com/decade-dnbc66/p/5347088.html> 仿函数
    ```
    #include <cstdio>
    #include <functional>

    class Foo{
    public:
      void operator()(int i){
        return bar(i);
      }
      void bar(int i){
        printf("%d\n", i);
      }
    };

    int main () {
      std::function<void(int)> f=Foo();
      f(2);
      return 0;
    }
    ```
    把类变成functor这种做法其实就是把类变成一个可调用对象，但是如果一个类有多个成员函数需要被回调的话，这个山寨的做法就没用了，那怎么办呢？这时候就是第二个组件bind出场的时候了。总的来说，bind的作用就是构造出一个新的可调用对象出来:
    ```
    #include <iostream>
    #include <cstdio>
    #include <memory>
    #include <functional>
    
    class Foo{
    public:
      void bar(int i){
        printf("%d\n", i);
      }
    };
    int main () {
      Foo foo;
      std::function<void(int)> f = std::bind( &Foo::bar, &foo, std::placeholders::_1 );
      f(3);
      return 0;
    }
    ```

- 模板函数




