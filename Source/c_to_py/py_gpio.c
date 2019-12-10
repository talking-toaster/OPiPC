#include <Python.h>
#include <stdio.h>
#include "gpiox_lib.c"

/*
*gpio配置标志
*0：Undistribute
*1：output
*[80:84] input    80:pulDisable   81:pullup    82:pull down      84:Reserved
*2:Multi-Func2
*3:Multi-Func3/Reserved
*4:Reserved
*5:Reserved
*6:Reserved/PA_int/PG_int
*7:IO-Disable
*/
unsigned char pin_conf_info[230];

/*
*初始化，通过mmap将PIO寄存器映射到内存中
*@param void
*@return NONE
*/
static PyObject *OPiPC_init(PyObject *self,PyObject *args){
    //printf("Py_INIT\n");
    if(sunxi_init()==-1){
        printf("Init error");
        return NULL;
    }
    //printf("py_init_end\n");
    Py_RETURN_NONE;
}

/*
*设置输出
*@param ：
*   pin:int   pinNum
*   mode:int
*       1:high
*       0:low
*@return NONE
*/
static PyObject *OPiPC_output(PyObject *self,PyObject *args){
    int pin,mode=0;
    //printf("Py_OUTPUT_prestart\n");
    
    if(!(PyArg_ParseTuple(args,"ii",&pin,&mode))){
        return NULL;
    }
    //printf("Py_OUTPUT_start  pin:%d  mode：%d\n",pin,mode);
    if (pin_conf_info[pin]!=1){
        if(sunxi_set_conf(pin,1)==-1){
            printf("Conf error\n");
            return NULL;
        }
        pin_conf_info[pin]=1;
    }
    //printf("Py_OUTPUT_mid\n");
    if(sunxi_output(pin,mode)<0){
        printf("Output error.\n");
        return NULL;
    }
    //printf("Py_OUTPUT_mid\n");
    Py_RETURN_NONE;
}

/*
*设置上拉/下拉
*@param ：
*   pin:int
*       pinNum
*   mode:int
*       0:Pull up/Pull down disable
*       1:pull up
*       2:pull down
*       3:Reversed
*@return NONE
*/
static PyObject *OPiPC_setPul(PyObject *self,PyObject *args){
    int pin,mode;
    //printf("Py_OUTPUT_prestart\n");
    
    if(!(PyArg_ParseTuple(args,"ii",&pin,&mode))){
        return NULL;
    }
    //printf("Py_OUTPUT_start  pin:%d  mode：%d\n",pin,mode);
    if(pin_conf_info[pin]<80 || pin_conf_info[pin]>84){
        if(sunxi_set_conf(pin,0)==-1){
            printf("Conf error\n");
            return NULL;
        }
        if(sunxi_pullup(pin,mode)==-1){
            printf("Set Pul error\n");
            return NULL;
        }
        pin_conf_info[pin]=80+mode;
    }else{
        if(sunxi_pullup(pin,mode)==-1){
            printf("Set Pul error\n");
            return NULL;
        }
        pin_conf_info[pin]=80+mode;
    }
    Py_RETURN_NONE;
}

/*
*获取一个gpio的输入
*@param ：
*   pin:int
*       pinNum
*@return int
*       1:high
*       0:low
*/
static PyObject *OPiPC_input(PyObject *self,PyObject *args){
    int pin;
    if(!(PyArg_ParseTuple(args,"i",&pin))){
        return NULL;
    }
    if(pin_conf_info[pin]<80 || pin_conf_info[pin]>84){
        if(sunxi_set_conf(pin,0)==-1){
            printf("Conf error.\n");
            return NULL;
        }
        if(sunxi_pullup(pin,0)==-1){
            printf("Set Pul error\n");
            return NULL;
        }
        pin_conf_info[pin]=80;
    }
    return (PyObject*)Py_BuildValue("i",sunxi_input(pin));
}

//为每个模块增加PyMethodDef ModuleMethods[]数组
static PyMethodDef OPiPCMethods[] = {
    {"init",OPiPC_init,METH_VARARGS},
    {"output",OPiPC_output,METH_VARARGS},
    {"input",OPiPC_input,METH_VARARGS},
    {"setPul",OPiPC_setPul,METH_VARARGS},
    {NULL,NULL},
};
 
static struct PyModuleDef OPiPCModule = {
    PyModuleDef_HEAD_INIT,
    "OPiPC",
    NULL,
    -1,
    OPiPCMethods
};
 
void PyInit_OPiPC(){
    PyModule_Create(&OPiPCModule);
}