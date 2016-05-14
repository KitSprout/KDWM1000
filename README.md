[KDWM1000](https://github.com/KitSprout/KDWM1000)
========
* Author  : [Hom](http://about.me/Hom)
* Version : v1.4
* Update  : 2016/05/14

Description
========
KDWM1000 是一款結合 [SmartIMU](https://github.com/Hom-Wang/SmartIMU)與 Decawave UWB DWM1000 模組的 Local Positioning System (LPS) 開發板，定位精度可達 10cm。

License
========
* 硬體(Hardware)採用 [CC BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW)  方式授權 
  
　　<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"><img alt="創用 CC 授權條款" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/tw/80x15.png" /></a>  
　　<span xmlns:dct="http://purl.org/dc/terms/" property="dct:title"> KDWM1000 v1.4 </span>由<a xmlns:cc="http://creativecommons.org/ns#" href="http://about.me/Hom" property="cc:attributionName" rel="cc:attributionURL"> Hom </a>製作，以<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.zh_TW"> 創用CC 姓名標示-相同方式分享 4.0 國際 授權條款 </a>釋出。  

* 軟體(Software)採用 [MIT License](http://opensource.org/licenses/MIT) 方式授權  

Hardware
========
* 控制器　 : [STM32F411C](http://www.st.com/web/en/catalog/mmc/FM141/SC1169/SS1577/LN1877/PF260148) 48Pin 100MHz DSP FPU
* 感測器　 : [MPU9250](http://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/) + [LPS25HB](http://www2.st.com/content/st_com/en/products/mems-and-sensors/pressure-sensors/lps25hb.html)
* UWB 　　 : DecaWave [DWM1000](http://www.decawave.com/products/dwm1000-module)
* PCB 尺寸 : 25.4 x 45.72 mm
* 設計軟體 [Altium Designer 16](http://www.altium.com/en/products/altium-designer) ( PcbLib use AD [PcbLib v2.5](https://github.com/KitSprout/AltiumDesigner_PcbLibrary/releases/tag/v2.5) )

Software
========
[KDWM_PeripheralSTD_ADC](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_ADC)  
[KDWM_PeripheralSTD_CRC](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_CRC)  
[KDWM_PeripheralSTD_EXTI](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_EXTI)  
[KDWM_PeripheralSTD_FLASH](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_FLASH)  
[KDWM_PeripheralSTD_GPIO](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_GPIO)  
[KDWM_PeripheralSTD_TIM](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_TIM)  
[KDWM_PeripheralSTD_UART](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_UART)  
[KDWM_PeripheralSTD_UART-IntRecv](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_PeripheralSTD_UART-IntRecv)  
[KDWM_ModuleSTD_MPU9250-SPI](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_ModuleSTD_MPU9250-SPI)  
[KDWM_ModuleSTD_LPS25HB-SPI](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_ModuleSTD_LPS25HB-SPI)  
[KDWM_ApplicationSTD_FreeRTOS](https://github.com/KitSprout/KDWM1000/tree/master/Software/KDWM_ApplicationSTD_FreeRTOS)  

Related Documents
========
* [Google Drive](https://goo.gl/hFYtBm)

View
========
<img src="https://lh3.googleusercontent.com/dBAGl-1shSu3bVKS5eRK00eV0uwWucbgiAS8wvxEeTY1azocywTnT1gnupPVx3tOUKeQLPplSga8IbEeMaX4k7IRDGTQDVinooAgjmmSHknrsHNZgz4Jhk13Cw_q1iuiRoP1oAVkT6nq1G9mhmfYgBX16h60F4uEdzMvAhtgsYD6aHX_csIOpmG3SFzDoViVdMF-dDEm5FkYocPaZKH9Of_RGLJPgRfTJOncwAT0Utk78KHS-xrMnI1KtC9rr80qindARlR5a19E9qbR0pkkz1hgKvRMZ82_q4d2cZFae5ogacnTUuvweYOVgkVpNMr8Bm_dhxbdvSMIQoG3AKWk2M1zGTZIWJ6e6MUzorN33wv7QjDtyeEMUzL5XCWr8OP0TDEWc6L6w9OCH63OT1J8xLKFVC2k_4o3dPZyfZeiyb-MhBpTHGi_lbr-yksfAuldBKSzfrJemWK81rEDcCvBjnP8tGwo6sxVM4Jqgm3x6x2_lmPXJi1tbZUa9gJbrJ8xr9Immp5w9uiZjyCkkyxlsSD41JOt_rfk6eQm0KcM_Fq54YLx8ED3LQ4t4FIqYDKs42LIJkMRdl_DGtDcXpAAq2AY8A6nsf4=w1034-h775-no"/>
<img src="https://lh3.googleusercontent.com/PEMUZbxsGQjh9mXY4DUa30DgvgArqD3I6iGT5cQM8a-nl2qnhzmubBHcUcTzONuR-2Uc5u3-f37YL40YrMU2CAgXo2ikNupfziBY3_mw2xgCLUm4WW9inIFHDi2pj3dEJxUsErODDoxi1tudU6jp83poipBO4uGaLhmRtbwmb01rTuaiZvmK81-jx76okV8dLA4QROyGtN2Y1xYyo--nWoucPfBtuoXnqnHDjTcT9oqOTmX3E8k1SEhXMmUF3oKWfzjsbXJblWGtNEdritYrfg9JVcs12ffLV1f8LDzKZNRYzaU49YG4CUKEbGAWGltlK2M2l6K4eQ455IlzLYSMQfWTEs4dye2o5XjmpALQKBjusDAwERxabN1RdiC4MS5XZDddRj79H1u60a1XFM9fiy9v-8bb3z8UQBrmunRwKqBy0J5iIpmyD6I9HbJsnGeMd5kFGZ7FtOEZSotEZ__vKH2GBJ_n0EGl264HENnSP1cTfbeIAl2SEB_2kSWWQ-yHVS29l4_OxaD0O01yAnxihBYXJOWzkP3f2GItu8gQtfeHOSho3a8x0nBD_QKO2ZiynItM0SXpYbt7M5dLciuyOkkgMDLgneQ=w952-h775-no"/>

<br />
[more photo...](https://goo.gl/photos/sF6GmofmAzrM65vT7)

Config
========
<img src="https://lh3.googleusercontent.com/UmZdUSpAXEuy0QbGTP-xQn7b7nQnqs4R4F27aYlREkYFF7WyPXajX6P3Bp3845o0tT8QSCH_qdDfHOsNQN6ADDyyJtPVSyfcFjXh455q3eHaw5sE42DYsKj-Ta1bNoYiwvv0A1CX3yG_XScOVHz-D4w-QwPEADneoW9WEyyV7CctmBNJQ9NUDu_RuRmcxZ9x3lBPBFX4WRJ14_TGVxG_tsjw6R6hSTxT-RPbHOAaFYkhL0hsa1nu_JsygrOlaANYl5JcljUwIMryMyle0cx3YJj-PZXtl-uVPlXNZSaEsVN3JmtEKBl2ZpiUo_AqCI14Xf8Rp2FWufrw9koQBnUC-RSadN_cpk8cO0KiMltR1hhBgAkrQsAf7C1tNBCniKhjZ14NvlydD84Tpnk3b5BH2_LRxkGCJt5TiBVDi5Fix_MkSI8yYyXd-3Do4k7MbNj_9g6ATkQknIfMqc7Gi1mnoTfhj1A5sWVYXB4oAJspUX7ANRyziAzSbvbV0LVIn9iXTHPqvDGOfa5H8szaqtBcBN8gXC06h18tt0l8YAYvfaI48VGWnFMMIubwCKI4RoQUOR5h3EgX1xD2fVzAqAYTEvbASeeBErw=w1440-h557-no"/>
<img src="https://lh3.googleusercontent.com/xQA4tronEl93KpkjpJSS-UP6tuwXoEqZ9FAzrTo4qZslZUXrR-TlS9ipuJTB7WRsFFZXTp7ozVS_KBLOqY84U3a1cnjZ5og_FXK_vOb943K1mrlccx0Y152u2LWBIreINeuZ476ylNHPInSthFeGmIvdEUSv3__weJhO7TB4Nt-dSpKC2hbX4WTjVM_aHez0uTNcVKVMNMmDkKY3amxeMD3wjeOarrw2zK3QPZiiXZp21vtOGT2vU0AyeCZjb5tXTRYLdOC6ZcaRVsxN809SmPKOGqGJ53xfJdM3ZmjtNwtjmozMQSSq0N22ZWZmwvO-qDL4rk22lZkUqrVb8LmjOqG_8LK_90AjdVRo2anoLr98wxAHsPbK3qdwjPzRTMbwUvafF1dOq_QYbmeSvIJp4a_NwBTToZYew5PCv6DqOf4XosvSZFXBOv8-gc7RafocHqGl93YWg_LvQv5nFJ2s_B8M8kffC3FMX9QQ-r0zHsVdUptTc3V-Roi1tizIo5WHPbnNjbFixxLqPmghD0_hRCKCk7tZBzCAlfWUuak86jdRzQq8WmGe7Z2jRpnXgUFgnbu-JD9uos5k6zUnxFM-7nenKOTQ75M=w1284-h558-no"/>

Schematic
========
<img src="https://lh3.googleusercontent.com/eV37jmym7pjf8tgiNgVrQST6ZoaoZWAyXJSGDw2C4VinDUYk4kG5W9--yvUqpccakwVRYNCtt4uu1kCC0kO42bWFsKxGtQNi7XOSGB7Fyqw-et-4FM91xP0Egd2BuS1adyuDARf2LMRGIXGnYpmZGyYkhe8H9UELZRiyiG8FI-aL42l5ye3e3Yw0JJid3M9WW1aQgd72imf69QsrRRAdv9Q8rEg5JtPzzRQrxsM8G0eFtb5goLzo8Mu5Ihg1RSE1H6HCsoFSgg79idIHFNm_6JeoQvYmOh2sN8Dh_B7td-DzQ48UaQm53THC_RN_Qb7y962V-8XL6F0OY4OPH5pjVyBTiL9nKNjvGhZlmO78hgcQIfI2EuNEPJqdCGm01yADzzybgOlRUCANgWIUdHFftzyMTLRlp4fWsaPq_F_-YQZwZJDfuPMt3lWCthbpH5tDCAEiN33OOVB6fr_ex_o3oCqMDu4MNhh4L7E8T2TjyJ_flLLc1MqC96pq8yWp_qe98xsYwoXlUNjvJqaqHUcA7MV2Oc0myXn9RJn9PIo3QpbRY8Q4XDSTCUCDt-D0KGkUBlrf4oVdDqm6P4Fy-J58IAi5C9puvBI=w1163-h775-no"/>
