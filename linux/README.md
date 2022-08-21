# 编译

`yqs_char_driver` 是一个简单的支持读写的字符驱动代码, 可以从该驱动循环读取 `YQS\n` 四个字符, 并支持任意的写入。

```
make
```

# 驱动管理

```bash
# 查看驱动
lsmod

# 安装驱动 
insmod yqs_char_driver.ko

# 查看已安装的驱动
cat /proc/devices
# 查看已安装的驱动
ls -l /dev/yqs_char_driver
# 查看内核输出(printk)
dmesg

# 删除驱动
rmmod yqs_char_driver
```

# 驱动使用

```bash
# 写入数据
echo "write 8" > /dev/yqs_char_driver
# 查看内核输出(printk)
dmesg

# 读出数据 循环输出 Ctrl+C 中断
cat /dev/yqs_char_driver
```
