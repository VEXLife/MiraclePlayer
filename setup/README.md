# 安装程序脚本

此目录包含用于创建安装程序所需的文件。

# 使用方法

1. 将编译好的程序使用windeployqt部署后重命名为miracleplayer_release，存放于仓库根目录下。
2. 选择一个你喜欢的安装程序制作软件（[Inno Setup](https://jrsoftware.org/isinfo.php)/[NSIS](https://nsis.sf.net)/[WiX Toolset](https://wixtoolset.org/)）
3. 安装相应软件，打开此目录下的对应文件编译即可。

## WiX Toolset配置说明

使用WiX Toolset你将得到一个msi安装包，然而配置过程较为繁琐。项目中需要4.0.0版本，我强烈推荐您按如下步骤配置更为简单：

1. 安装Visual Studio，您需要勾选“使用C++的桌面开发”（若您按照[主README.md](../README.md)配置过开发环境，那么该工作负载可能已经勾选）和“.NET桌面开发”。
2. 下载[FireGiant HeatWave](https://www.firegiant.com/wix/heatwave/)，作为Visual Studio扩展安装。或者前往Visual Studio Marketplace搜索下载安装。

搞定！

## WiX Toolset构建

您可以用Visual Studio打开此目录下的[mplayer_setup_wix.wixproj](mplayer_setup_wix.wixproj)，然后将构建配置改为x64进行编译（因为奇迹音乐默认不支持32位，您也可以以32位模式也就是x86重新编译奇迹音乐，然后在这步改为x86编译）。或者，在终端（也可以是命令提示符或Powershell）运行：

```bash
$ dotnet build
```

如此您便能得到所需的msi安装包。