# 编写接口的benchmark单测函数
比如

```golang
func BenchmarkDebug(b *testing.B) {
    b.Run("api", func(b *testing.B) {
        for i := 0; i < b.N; i++ {
            logger.Debug("xxx")
        }
    })
}
```

# 运行单测
使用如下命令运行单测，其中：
- -bench：指定运行的benchmark单测函数
- -count=1：单测函数多次执行，后续会直接返回缓存的结果，这里是指定忽视缓存，确保单测函数一定会执行
- -benchmem：展示单测函数中分配的内存信息
- -cpuprofile：收集单测函数的cpu pprof数据，用于生成火焰图
- -memprofile：收集单测函数的内存 pprof数据，用于生成火焰图
- -benchtime：运行benchmark的时长，一般10s就足够了
```
go test -benchmem -run=^$ -bench ^BenchmarkDebug$ -v -count=1 -benchmem -cpuprofile=cpu.prof -memprofile=mem.prof -benchtime 5s
```
运行结果如下，单测函数的数据分别是
- 循环次数
- 单次执行的平均耗时
- 单次执行的平均分配内存
- 单次执行的平均分配次数

```
BenchmarkDebug
BenchmarkDebug/api
BenchmarkDebug/api-8                 1234              1234 ns/op             1234 B/op         1234 allocs/op
```

# 解析pprof数据
使用go tool命令解析pprof数据，并使用浏览器打开
```shell
go tool pprof -http=:1234 ./cpu.prof
go tool pprof -http=:1235 ./mem.prof
```
