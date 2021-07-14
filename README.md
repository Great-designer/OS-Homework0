# OS-Homework0

操作系统实验0。以下是原README文件的翻译。

此包包含cs140-ps0启动程序代码。

包含的Makefile将编译caltrain和reaction同步问题的解决方案。

生成的程序将对您编写的内容进行压力测试，希望能够帮助您摆脱bug。

你需要适当地填写caltrain.c和reaction.c来完成这项任务（在此之前测试肯定会失败）。

需要提交评分的有且只有这两个文件，有关详细信息请访问网站。

您将使用Pintos锁和条件变量API。已定义以下类型供您使用：

    struct lock
    struct condition

它们的功能如下：

    void lock_init(struct lock *lock);
    void lock_acquire(struct lock *lock);
    void lock_release(struct lock *lock);
    void cond_init(struct condition *cond);
    void cond_wait(struct condition *cond, struct lock *lock);
    void cond_signal(struct condition *cond, struct lock *lock);
    void cond_broadcast(struct condition *cond, struct lock *lock);

有关这些同步原语的更多信息，请参阅Pintos文档。

不需要修改其他文件。使用“make”编译。要运行一系列测试，可以尝试“make run”。

这些程序已经在Myth Linux集群和OSX上进行了测试，对于其他的类UNIX的操作系统而言，应该是非常可移植的。

这些测试有很多随机性，并不是完美无缺的（通过测试不能保证实现的完美）。

为了更好地保证解决方案的合理性，您可能需要运行多次。

欢迎您以任何您认为合适的方式扩展测试，但它们无需提交。

您应该避免在解决方案中使用任何其他库或函数。

我们应该能够解压您的提交，复制caltrain.c和reaction.c文件，编译并运行原始测试。

注意，硬超时用于捕捉死锁之类的问题。

在繁忙的公用计算机上（或者可能是非常落后的上网笔记本）它们可能过于落后。

如果你确信你的代码是正确的，而测试仍然没有通过，那就在办公时间来，我们会尽力帮助你找出问题所在。
