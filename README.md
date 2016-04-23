[KDWM1000](https://github.com/KitSprout/KDWM1000)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v1.2
* Update  : 2016/04/23

Description
========
KDWM1000 是一款結合 UWB 與 [SmartIMU](https://github.com/Hom-Wang/SmartIMU) 的 Local Positioning System (LPS) 開發板。

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> KDWM1000 v1.2 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F411C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](http://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS25HB](http://www2.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps25hb.html)
* UWB 　　 : DecaWave [DWM1000](http://www.decawave.com/products/dwm1000-module)
* PCB 尺寸 : 25.4 x 45.72 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.4](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.4) )

Software
========
[KDWM_PeripheralSTD_GPIO](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_GPIO)

Related Documents
========
* [Google Drive](https://goo.gl/hFYtBm)

View
========
<img src="https://lh3.googleusercontent.com/hfC1cY4f4LyEm6jocEopPyitupuz2SWdHpx5pG51UHOzPzyu-IGUJnctck7kuXDpLfTiEJJiEDXWHV5DbATZtIri9zSbusFLophfJ7hVGYWIHMSJ9Hy099zwAS37Q_0nwLa-17BNRTCxRA-VPDLqutu5mftSmFSlO-cxJCt6gsQ00qWboiaO3YB1US9cXxu-j3MTbH3XZGEX6IgFHvNLhNiPweBx3UeRM3pBpfvwLJVKvxyOESgugQhG-VGzjeYmKFvUXjtHAwUPh3LHRq90zV1FizdxOLDbGmEvjwx-9G5iL-7a1lpZ-CiErsBIrtGRFm2YUBXNKWnmaNPwOjYpGswn3qNICKtXoJmJHh0CJvHbjHdQ4ZVM8JEqTaOrZ-BMIEbBwvsklNNbn_WKbCnvJRE09Y2WogaFaVAp5xVvNNIKashshaBD7yApL1pEOBFnbln5w5Q3Eod2T6fJrpJw2vnMcmbL4ctP5JghqqEOGVe9IYqM02oIgfLWwhBb3SeefpnbZx7ggmWNaWOPfQftVsjaG4W73uzx-rgcbWOmMtO0wOB2iRkkSTZM_rD7URN5U24Y=w991-h775-no"/>

<br />
[more photo...](https://goo.gl/photos/sF6GmofmAzrM65vT7)

Config
========
<img src="https://lh3.googleusercontent.com/xCVi7yjRGhEWsW5hN0hOJbLEO5eyE8FvC_wav410RDv0N2I7RuzqLy6p3jOu08zG8zOZlFNdvKbBAmK4KOWqAbwnETNmyc4i9QMbXlnXIVu99clu1I4kYOZugItqZOXkhlYbGcLEurllwqxy6pGfa6YMQXP227c1cdGznX2bu-LPf6lbwzfYpCW4IBbkHJA4KuwN4pOV0vfxemP7yGfsr56lHVrl49lXzJx_Bu40oMHc-5uBoWEWB7WAP9oktrBmrb751Dcc3PvyTkPa56h2ZopwWr1Fu83dqqRQfRcKdx5zVVhl_4Xy90JadQibl9Vs1D3hRuIAp2XSkZOoWqVDGDHI_63szo6v5wpfj3EeRK3CzPULngH03ywA_FTEHePwNjhZkN-F8bXPfrwiixvIl-dQHlkdBFoSZpdxKvVt7BrRQTK7TARiIHi0vDcy6BVea1uoRDXoqxyqDGLwIm_whPRqjp2gc4BHgOsQJODTy0jq6lDJFFQANZSoJMxPCnYcEwtz3Ctks5lHdLq9ELIp_Vys79Qu14O5GW3e-dx1yGQKcqMROuCSS7IgCZtwihlJAzwH=w1440-h560-no"/>

Schematic
========
<img src="https://lh3.googleusercontent.com/srYciezWWgj7UO6zeaKSZprF6yrIWIOpOql5pTnG4TSxYeIAcjB3aMwCPkHL1hoo_qxz3Su7dFqUvReIsTP-RQpygJn6Lnowb6hBGMD-mph-4iDu7woOKTyBV9D_jSsO0DonAHIlCIXr3flObJIrqkLOY4y4NtfASDefIZ27d6U1Gk5PDj3YscpOQ_RscrozV4KT8iJiQyFVTxNlhI4bkxqZxLyGr9jZAMDD6V2TCg5Do26hOqovKVYva-YRPkxzci5POtfPHq5Jdv82P_mP3aS1JRHE5NzDsYRddoPTIiEgIJN8cfvsy8iS5tEYq84IuGi8FKyeh72NlxwLKxrB4-8WbL9vj8lzyCtjKKv4uMOs2kii2TAEZmlP_xqviopdqmeYqvwV0opS6qf4MRB-Nic1RURz55krI6QgzHnnzV6iCB_OgSRbKQQ1ce3qFXUdVo7byc8MLU8RnXDEtwbRUjClYdO8VcVV6FwlSQ8k5Oic2DpiSOmxiVTpt3tbdrgkENpC3vIQf-s7ztxN8HcCCh4xNzOBqKraoWaMxU-25bb3OnoTnmWd80OjJXaTuEdboc87=w1745-h1163-no"/>
