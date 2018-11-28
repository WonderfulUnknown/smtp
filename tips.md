# study
CSocket类的函数工作在阻塞的模式，一旦调用send()，所有数据发送前，程序和线程都被阻塞。一般将CSocket与CArchive和CSocketFile结合，用来发送和接受数据。
# unknown
```
#if !defined(AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_)
#define AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_
#endif

#if _MSC_VER > 1000
#pragma once
#endif
```