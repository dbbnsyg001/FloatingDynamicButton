# FloatingDynamicButton
qt项目：悬浮的动态阴影按钮

请遵守MIT开源协议，使用时勿删除原作者信息


### 效果
这是一个动态的悬浮按钮，普通状态如下，支持拖动和限制范围  
![动画1](https://user-images.githubusercontent.com/48360609/236614949-3bb201bd-5f74-434e-b45e-a328ca45dcd2.gif)


另外你可以给他设置中心按钮的图标和文字  
![动画2](https://user-images.githubusercontent.com/48360609/236776900-725c9642-37d7-4775-bfa0-314e67d07139.gif)


### 优势
1.动态阴影可自定决定是否渐变刷新  
1.动态阴影由单独定时器和刷新函数组成，不受拖动时由于QT刷新机制导致的停顿影响  
2.组件解决了按钮穿透机制，可自由嵌入或者悬浮在任意组件上  
3.可以指定悬浮控件拖动范围，指定父类只能在父类范围内拖动，不指定父类拖动范围是整个桌面  
4.所有参数都实现了自定义，刷新速率，动态阴影大小，阴影颜色和边框等，修改参数可直接调整  
![image](https://user-images.githubusercontent.com/48360609/236777086-65bd668a-85da-4c19-a414-ee0aba0b69fb.png)

![image](https://user-images.githubusercontent.com/48360609/236777126-3be58a58-adb6-4e05-b823-fc9e2dd471ec.png)


### 项目概念结构

最外层是BaseDragLimitWidget，最内层是一个按钮，使用按钮的原因是该控件点击时抛出信号，方便外部接收
![结构](https://user-images.githubusercontent.com/48360609/236615359-743db86f-4c2b-4019-8ff0-7a9afa5d84c1.png)

### 未完善问题
由于中心的按钮FloatingInsideButton继承的是QT普通的QPushbutton，有以下问题  
1.中心文字过长时，需要缩略显示并显示tooltip，普通QPushbutton不带该功能  

#### 以下部分涉及个人使用所以不做开源，仅提供思路：
1.建议使用者继承QPushbutton自己重写setText，和tooltip等相关刷新，让FloatingInsideButton继承该类，可实现更精致定制化的UI和tooltip显示


### LICENSE
``` 
MIT License

Copyright (c) 2023 凝视阳光 dbbnsyg001

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
``` 
