# spectre 漏洞利用

利用cmake进行编译

其中char *target = "flxg:{Deep Dark Fantasy}"为默认的被攻击字符串，可以任意修改

取消#define DEBUG注释会显示更详细的结果

程序会使用访问时间来确定字符串内容，并给出可能的二义性字符

非debug模式下，输出结果可能是这样：

```
get answer : flxg:{Deep Dark Fantasy}
```



也有小概率会是这样：

```
There may be ambiguity for [y] and [ ] at 22

get answer : flxg:{Deep Dark Fantasy}
```



