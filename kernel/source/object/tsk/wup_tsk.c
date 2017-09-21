/** 
 *  Hyper Operating System V4 Advance
 *
 * @file  wup_tsk.c
 * @brief %jp{タスクの起床}%en{Wakeup Task}
 *
 * Copyright (C) 1998-2009 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */



#include "core/core.h"


#if _KERNEL_SPT_WUP_TSK


/** %jp{タスクの起床}%en{Wakeup Task}
 * @param  tskid    %jp{起床対象のタスクのID番号}%en{ID number of the task to be woken up}
 * @retval E_OK     %jp{正常終了}%en{Normal completion}
 * @retval E_ID     %jp{不正ID番号(tskidが不正あるいは使用できない)}%en{Invalid ID number(tskid is invalid or unusable)}
 * @retval E_NOEXS  %jp{オブジェクト未生成(対象タスクが未登録)}%en{Non-existant object(specified task is not registered)}
 * @retval E_OBJ    %jp{オブジェクト状態エラー(対象タスクが休止状態)}%en{Object state error(specified task is in DORMANT state)}
 * @retval E_QOVR   %jp{キューイングオーバーフロー(起床要求キューイングのオーバーフロー)}%en{Queue overflow(overflow of wakeup request count)}
 */
ER wup_tsk(
		ID tskid)
{
	_KERNEL_T_TSKHDL tskhdl;
	_KERNEL_T_TCB    *tcb;
	
	if ( tskid == TSK_SELF )	/* %jp{自タスク指定時の変換} */
	{
#if _KERNEL_SPT_WUP_TSK_E_ID
		if ( _KERNEL_SYS_SNS_CTX() )
		{
			return E_ID;		/* %jp{不正ID番号}%en{Invalid ID number} */
		}
#endif	
		/* %jp{実行中タスクを取得} */
		tskhdl = _KERNEL_SYS_GET_RUNTSK();
		
		_KERNEL_ENTER_SVC();	/* %jp{サービスコールに入る}%en{enter service-call} */
	}
	else
	{
#if _KERNEL_SPT_WUP_TSK_E_ID
		if ( !_KERNEL_TSK_CHECK_TSKID(tskid) )
		{
			return E_ID;	/* %jp{不正ID番号} */
		}
#endif
		
		_KERNEL_ENTER_SVC();			/* %jp{サービスコールに入る}%en{enter service-call} */
		
		/* %jp{オブジェクト存在チェック} */
#if _KERNEL_SPT_WUP_TSK_E_NOEXS
		if ( !_KERNEL_TSK_CHECK_EXS(tskid) )
		{
			_KERNEL_LEAVE_SVC();		/* %jp{サービスコールを出る}%en{leave service-call} */
			return E_NOEXS;				/* %jp{オブジェクト未生成}%en{Non-existant object} */
		}
#endif
		/* %jp{ID番号指定時の変換} */
		tskhdl = _KERNEL_TSK_ID2TSKHDL(tskid);
	}
	
	/* %jp{TCB取得} */
	tcb = _KERNEL_TSK_TSKHDL2TCB(tskhdl);
	
	if ( ((_KERNEL_TSK_GET_TSKSTAT(tcb) & _KERNEL_TTS_WAI)) && _KERNEL_TSK_GET_TSKWAIT(tcb) == _KERNEL_TTW_SLP )
	{
		/* %jp{待ち解除} */
		_KERNEL_TSK_SET_ERCD(tcb, E_OK);		/* %jp{エラーコード設定} */
		_KERNEL_DSP_WUP_TSK(tskhdl);			/* %jp{タスクの待ち解除} */
		
		/* タスクディスパッチの実行 */
		_KERNEL_DSP_TSK();
	}
	else
	{
#if _KERNEL_TCB_WUPCNT
		{
			_KERNEL_TSK_T_WUPCNT wupcnt;
			
			wupcnt = _KERNEL_TSK_GET_WUPCNT(tcb);

#if _KERNEL_SPT_WUP_TSK_E_QOVR
			if ( wupcnt >= _KERNEL_TMAX_WUPCNT )
			{
				_KERNEL_LEAVE_SVC();	/* %jp{サービスコール終了} */
				return E_QOVR;
			}
#endif
			
			_KERNEL_TSK_SET_WUPCNT(tcb, wupcnt + 1);
		}
#endif
	}

	_KERNEL_LEAVE_SVC();	/* %jp{サービスコール終了} */

	return E_OK;
}


#else	/* _KERNEL_SPT_WUP_TSK */

/** %jp{タスクの起床}%en{Wakeup Task}
 * @param  tskid    %jp{起床対象のタスクのID番号}%en{ID number of the task to be woken up}
 * @retval E_NOSPT  %jp{未サポート機能}%en{Unsupported function}
 */
ER wup_tsk(
		ID tskid)
{
	return E_NOSPT;
}

#endif	/* _KERNEL_SPT_WUP_TSK */


/* end of file */
