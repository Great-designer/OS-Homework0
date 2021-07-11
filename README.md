## OS-Homework0

操作系统实验0。以下是原版README文件的内容，带翻译。

This package contains cs140 ps0 starter code. The included Makefile
will compile your solutions to the caltrain and reaction synchronisation
problems. The resulting binaries will try to stress-test what you
wrote and should hopefully help you to shake out any bugs.

此包包含cs140 ps0启动程序代码。
包含的Makefile将编译caltrain和reaction同步问题的解决方案。
生成的二进制文件将尝试对您编写的内容进行压力测试，希望能够帮助您摆脱任何bug。

You'll need to fill in caltrain.c and reaction.c appropriately to
complete this assignment (the tests will certainly fail before you do
so). These two files, and only these two files, should be submitted for
grading. See the website for forthcoming submission details.

你需要适当地填写caltrain.c和reaction.c来完成这项任务
（在你这样做之前，测试肯定会失败）。
这两个文件，而且只有这两个文件，应提交评分。
有关即将提交的详细信息，请访问网站。

You are to use the Pintos lock and condition variable APIs. The following
types are defined already for your use:

您将使用Pintos锁和条件变量api。
已定义以下类型供您使用：

    struct lock
    struct condition

As are the following functions:

其功能如下：

    void lock_init(struct lock *lock);
    void lock_acquire(struct lock *lock);
    void lock_release(struct lock *lock);
    void cond_init(struct condition *cond);
    void cond_wait(struct condition *cond, struct lock *lock);
    void cond_signal(struct condition *cond, struct lock *lock);
    void cond_broadcast(struct condition *cond, struct lock *lock);

See the Pintos documentation for more information on these synchronisation
primitives.

有关这些同步原语的更多信息，请参阅Pintos文档。

No other files should need to be modified. Compile using 'make'. To
run a bunch of tests, you can try 'make run'. These programs have been
tested on the myth linux cluster and OS X and should be pretty portable to
other UNIX-like OSes.

不需要修改其他文件。使用“make”编译。
要运行一系列测试，可以尝试“make run”。
这些程序已经在MythyLinux集群和OSX上进行了测试，对于其他类似UNIX的操作系统来说应该是非常可移植的。

The tests have a lot of randomness built in and aren't flawless (their
passing doesn't guarantee a perfect implementation). You may want to
run many times to get better assurance of your solution's sanity. You're
welcome to extend the tests any way you see fit, but they won't be
submitted. You should refrain from using any other libraries or functions
in your solutions. We should be able to untar this distribution, copy over
your caltrain.c and reaction.c files, and compile and run the original
tests.

这些测试有很多随机性，并不是完美无缺的
（它们的通过并不能保证完美的实现）。
为了更好地保证解决方案的合理性，您可能需要运行多次。
欢迎您以任何您认为合适的方式扩展测试，但它们不会被提交。
您应该避免在解决方案中使用任何其他库或函数。
我们应该能够解压这个发行版，复制caltrain.c和reaction.c文件，编译并运行原始测试。

Note that hard timeouts are used to catch some issues like deadlock.
It's possible that on busy shared machines (or perhaps really anemic
netbooks) they're too conservative. If you're dead sure your code is
correct and tests still don't pass, just come by office hours and
we'll try to help you figure out what's going wrong.

注意，硬超时用于捕捉死锁之类的问题。
可能是在繁忙的共享计算机上（或者可能是真的贫血的上网本）他们太保守了。
如果你确信你的代码是正确的，而测试仍然没有通过，那就在办公时间来，我们会尽力帮助你找出问题所在。
