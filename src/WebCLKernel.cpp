/*
** This file contains proprietary software owned by Motorola Mobility, Inc. **
** No rights, expressed or implied, whatsoever to this software are provided by Motorola Mobility, Inc. hereunder. **
**
** (c) Copyright 2011 Motorola Mobility, Inc.  All Rights Reserved.  **
*/

#include "WebCL.h"
#include "WebCLKernel.h"
#include "WebCLProgram.h"
#include "WebCLMemory.h"
#include "WebCLContext.h"
#include "WebCLDevice.h"
#include "WebCLPlatform.h"

#include <iostream>
using namespace std;

using namespace v8;
using namespace webcl;
using namespace cl;

Persistent<FunctionTemplate> WebCLKernel::constructor_template;

/* static  */
void WebCLKernel::Init(Handle<Object> target)
{
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(WebCLKernel::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);

  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("WebCLKernel"));

  NODE_SET_PROTOTYPE_METHOD(constructor_template, "getInfo", getInfo);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "getWorkGroupInfo", getWorkGroupInfo);
  NODE_SET_PROTOTYPE_METHOD(constructor_template, "setArg", setArg);

  target->Set(String::NewSymbol("WebCLKernel"), constructor_template->GetFunction());
}

WebCLKernel::WebCLKernel(Handle<Object> wrapper) : kernel(0)
{
}

WebCLKernel::~WebCLKernel()
{
  if (kernel) delete kernel;
}

/* static */
JS_METHOD(WebCLKernel::getInfo)
{
  HandleScope scope;
  WebCLKernel *kernelObject = ObjectWrap::Unwrap<WebCLKernel>(args.This());
  cl_kernel_info param_name = args[0]->Uint32Value();

  switch (param_name) {
  case CL_KERNEL_FUNCTION_NAME: {
    STRING_CLASS param_value;
    cl_int ret=kernelObject->getKernel()->getInfo(param_name,&param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }
    return scope.Close(JS_STR(param_value.c_str(),param_value.length()));
  }
  case CL_KERNEL_CONTEXT: {
    cl::Context *param_value=new cl::Context();
    cl_int ret=kernelObject->getKernel()->getInfo(param_name,param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }
    return scope.Close(WebCLContext::New(param_value)->handle_);
  }
  case CL_KERNEL_PROGRAM: {
    cl::Program *param_value=new cl::Program();
    cl_int ret=kernelObject->getKernel()->getInfo(param_name,param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }
    return scope.Close(WebCLProgram::New(param_value)->handle_);
  }
  case CL_KERNEL_NUM_ARGS:
  case CL_KERNEL_REFERENCE_COUNT: {
    cl_uint param_value=0;
    cl_int ret=kernelObject->getKernel()->getInfo(param_name,&param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }
    return scope.Close(JS_INT(param_value));
  }
  default:
    return JS_EXCEPTION("UNKNOWN param_name");
  }
}

/* static */
JS_METHOD(WebCLKernel::getWorkGroupInfo)
{
  HandleScope scope;
  WebCLKernel *kernelObject = ObjectWrap::Unwrap<WebCLKernel>(args.This());
  WebCLDevice *device = ObjectWrap::Unwrap<WebCLDevice>(args[0]->ToObject());
  cl_kernel_work_group_info param_name = args[1]->NumberValue();

  switch (param_name) {
  case CL_KERNEL_WORK_GROUP_SIZE:
  case CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE: {
    std::size_t param_value=0;
    cl_int ret=kernelObject->getKernel()->getWorkGroupInfo(*device->getDevice(), param_name, &param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_DEVICE);
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }
    return scope.Close(JS_INT(param_value));
  }
  case CL_KERNEL_LOCAL_MEM_SIZE:
  case CL_KERNEL_PRIVATE_MEM_SIZE: {
    cl_ulong param_value=0;
    cl_int ret=kernelObject->getKernel()->getWorkGroupInfo(*device->getDevice(), param_name, &param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_DEVICE);
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }
    return scope.Close(JS_INT(param_value));
  }
  case CL_KERNEL_COMPILE_WORK_GROUP_SIZE: {
    std::size_t param_value[]={0,0,0};
    cl_int ret=kernelObject->getKernel()->getWorkGroupInfo(*device->getDevice(), param_name, &param_value);
    if (ret != CL_SUCCESS) {
      REQ_ERROR_THROW(CL_INVALID_DEVICE);
      REQ_ERROR_THROW(CL_INVALID_VALUE);
      REQ_ERROR_THROW(CL_INVALID_KERNEL);
      REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
      REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
      return JS_EXCEPTION("UNKNOWN ERROR");
    }

    Local<Array> sizeArray = Array::New(3);
    for (std::size_t i=0; i<3; i++) {
      sizeArray->Set(i, JS_INT(param_value[i]));
    }
    return scope.Close(sizeArray);
  }
  default:
    return JS_EXCEPTION("UNKNOWN param_name");
  }
}

/* static */
JS_METHOD(WebCLKernel::setArg)
{
  HandleScope scope;

  if (!args[0]->IsUint32())
    return JS_EXCEPTION("CL_INVALID_ARG_INDEX");

  WebCLKernel *kernelObject = ObjectWrap::Unwrap<WebCLKernel>(args.This());
  cl_uint arg_index = args[0]->Uint32Value();
  std::size_t arg_size = 0;
  cl_uint type = 0;
  void *arg_value = 0;
  cl_int ret;

  type = args[2]->Uint32Value();

  // TODO check types other than MEMORY_OBJECT
  if(type & webcl::types::LOCAL) { // TODO is it true for all __local args?
    cl_float arg = args[1]->NumberValue();
    arg_value = NULL;
    arg_size = sizeof(arg);
    ret = kernelObject->getKernel()->setArg(arg_index, arg_size, arg_value);
  }
  else if(type & webcl::types::MEM) {
    cl_mem mem;
    if (args[1]->IsUint32()) {
      cl_uint ptr = args[1]->Uint32Value();
      if (ptr)
        return JS_EXCEPTION("ARG is not of specified type");
      mem = 0;
    } else {
      WebCLMemory *mo = ObjectWrap::Unwrap<WebCLMemory>(args[1]->ToObject());
      mem = mo->getMemory()->operator ()();
    }
    arg_value=&mem;
    arg_size=sizeof(mem);
    ret = kernelObject->getKernel()->setArg(arg_index, arg_size, arg_value);
  }
  else if(type & webcl::types::INT) {
    if (((type & webcl::types::UNSIGNED) && !args[1]->IsUint32()) || !args[1]->IsInt32())
      return JS_EXCEPTION("ARG is not of specified type");
    if(type & webcl::types::UNSIGNED) {
      cl_uint arg=args[1]->Uint32Value();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
    else {
      cl_int arg=args[1]->Int32Value();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
  }
  else if(type & webcl::types::LONG) {
    if (!args[1]->IsNumber())
      return JS_EXCEPTION("ARG is not of specified type");
    if(type & webcl::types::UNSIGNED) {
      cl_ulong arg = args[1]->NumberValue();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
    else {
      cl_long arg = args[1]->NumberValue();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
  }
  else if(type && webcl::types::FLOAT) {
    if (!args[1]->IsNumber())
      return JS_EXCEPTION("ARG is not of specified type");
    cl_float arg = args[1]->NumberValue();
    arg_value = &arg;
    arg_size = sizeof(arg);
  }
  else if(type & webcl::types::HALF_FLOAT) {
    if (!args[1]->IsNumber())
      return JS_EXCEPTION("ARG is not of specified type");
    cl_half arg = args[1]->NumberValue();
    arg_value = &arg;
    arg_size = sizeof(arg);
  }
  else if(type & webcl::types::SHORT) {
    if (!args[1]->IsNumber())
      return JS_EXCEPTION("ARG is not of specified type");
    if(type & webcl::types::UNSIGNED) {
      cl_ushort arg = args[1]->NumberValue();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
    else {
      cl_short arg = args[1]->NumberValue();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
  }
  else if(type & webcl::types::CHAR) {
    if (!args[1]->IsNumber())
      return JS_EXCEPTION("ARG is not of specified type");
    if(type & webcl::types::UNSIGNED) {
      cl_uchar arg = args[1]->NumberValue();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
    else {
      cl_char arg = args[1]->NumberValue();
      arg_value = &arg;
      arg_size = sizeof(arg);
    }
  }
  else {
    return JS_EXCEPTION("UNKNOWN TYPE");
  }

  ret = kernelObject->getKernel()->setArg(arg_index, arg_size, arg_value);
  if (ret != CL_SUCCESS) {
    REQ_ERROR_THROW(CL_INVALID_KERNEL);
    REQ_ERROR_THROW(CL_INVALID_ARG_INDEX);
    REQ_ERROR_THROW(CL_INVALID_ARG_VALUE);
    REQ_ERROR_THROW(CL_INVALID_MEM_OBJECT);
    REQ_ERROR_THROW(CL_INVALID_SAMPLER);
    REQ_ERROR_THROW(CL_INVALID_ARG_SIZE);
    REQ_ERROR_THROW(CL_OUT_OF_RESOURCES);
    REQ_ERROR_THROW(CL_OUT_OF_HOST_MEMORY);
    return JS_EXCEPTION("UNKNOWN ERROR");
  }

  return Undefined();
}

/* static  */
JS_METHOD(WebCLKernel::New)
{
  HandleScope scope;
  WebCLKernel *cl = new WebCLKernel(args.This());
  cl->Wrap(args.This());
  return scope.Close(args.This());
}

/* static  */
WebCLKernel *WebCLKernel::New(cl::Kernel* kw)
{

  HandleScope scope;

  Local<Value> arg = Integer::NewFromUnsigned(0);
  Local<Object> obj = constructor_template->GetFunction()->NewInstance(1, &arg);

  WebCLKernel *kernel = ObjectWrap::Unwrap<WebCLKernel>(obj);
  kernel->kernel = kw;

  return kernel;
}
