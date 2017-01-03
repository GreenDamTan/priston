
#define _EVENT_GAME_
//#define _EVENT_GAME_STATE_INIT
//#define _EVENT_GAME_CHUSUK_GIFT
//#define _EVENT_GAME_ATTENDANCE
//#define _EVENT_PCBANG_ATTENDANCE_
//#define _EVENT_GAME_SEOLLAL_GIFT
//#define _EVENT_GAME_CHRISTMAS_

#ifdef _EVENT_GAME_

#define NPC_NUM			2
#define REVEDUSER_MAX	5000

static int rsFlag_rsFiltChatMsg = 0;

struct rsEVENT_REVEDUSER {
	DWORD	dwIDCode;
	char	szID[32];
};

static int rsHopeEventNpcPos[][3] = {
    //field, x��ǥ,  z��ǥ  
	///// PC��, �⼮, ��й� �̺�Ʈ�Ҷ��� ��ǰ ����ó NPC�� ����Ѵ�.
	{ 3 , 2419 , -16324 }, // : ũ����  [0] ��ī����
	{ 0 , 0 , 0 }, // : ũ����  [1]
	{ 0,0,0 }               //						  [2]

};


// �庰 - ��й� �̺�Ʈ
extern char *StateInitEvent1;
extern char *StateInitEvent2;
extern char *StateInitEvent3;
extern char *StateInitEvent4;


	
	///////////////////////////////////////// ����� : ��й� �̺�Ʈ ///////////////////////////////////////////////////////    // �庰 - ��й� �̺�Ʈ 
	#ifdef _EVENT_GAME_STATE_INIT
	extern char *szEvent_HopeMsg_StateInit;  // "��й� �ϰ� �;��"  // "���ʵ� �����Ϸ� �Ծ��" - �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ

	rsEVENT_REVEDUSER rsEventUser_RecedList_StateInit[REVEDUSER_MAX];
	int rsEventUser_RecvedCount_StateInit = 0;
	#endif



#endif


// �������� �����ȿ��� Ư�� ���ڿ� ���͸�
int sveFiltChatMsg( rsPLAYINFO *lpPlayInfo , char *szMsg , int len )
{
#ifdef _EVENT_GAME_

	int x=0;
	int z=0;
	int dist=0;
	int cnt=0;


	#ifdef _EVENT_GAME_STATE_INIT  // ��й� �̺�Ʈ // �庰 - ��й� �̺�Ʈ
	cnt = 0;	// �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ
	while(1){
		if ( rsHopeEventNpcPos[cnt][0]==0 ) break;
		if ( lpPlayInfo->Position.Area==rsHopeEventNpcPos[cnt][0] ) 
		{
			x = abs(lpPlayInfo->Position.x/fONE - rsHopeEventNpcPos[cnt][1]);
			z = abs(lpPlayInfo->Position.z/fONE - rsHopeEventNpcPos[cnt][2]);
			dist = x*x+z*z;
			if ( dist<150*150 ) 
			{
				if ( strstr( szMsg , szEvent_HopeMsg_StateInit )>0 ) 
				{
					smTRANS_COMMAND smTransCommand;
					smTransCommand.size = sizeof(smTRANS_COMMAND);
					smTransCommand.code = smTRANSCODE_EVENT_GAME;
					smTransCommand.WParam = smCODE_EVENT_STATE_INIT;
					smTransCommand.SParam = lpPlayInfo->Position.Area;
					smTransCommand.LParam = rsServerConfig.ServerCode;
					rsSendDataServer( lpPlayInfo->lpsmSock , &smTransCommand );  // �α��� ������ �����ش�
					break;
				}
			}
		}
		cnt++;
	}
	#endif // ��й� �̺�Ʈ
	

	


#endif

	return TRUE;
}




// ��й� ���� �ʱ�ȭ(��罺�� ������ ����)
int rsEventGame_State_Init (rsPLAYINFO *lpPlayInfo , smTRANS_COMMAND *lpTransCommand)
{
#ifdef _EVENT_GAME_STATE_INIT

	SYSTEMTIME	st;
	GetLocalTime( &st );
/*
	if ( rsServerConfig.TestSeverMode ) 
	{
		if ( lpPlayInfo->AdminMode<2 && (st.wYear!=2009 || st.wMonth!=4 || st.wDay<16 || st.wDay>22) ) //�̺�Ʈ ���� ( ��¥ ���ǿ� �¾ƾ� �� )
			return FALSE;
	}
	else 
	{
		if ( lpPlayInfo->AdminMode<2 && (st.wYear!=2009 || st.wMonth!=4 || st.wDay<16 || st.wDay>22) ) //�̺�Ʈ ���� ( ��¥ ���ǿ� �¾ƾ� �� )
			return FALSE;
	}
*/	

	TRANS_CHATMESSAGE	TransChatMessage;
	int cnt;

	if ( rsEventUser_RecvedCount_StateInit>=REVEDUSER_MAX ) { // �庰 - ��й� �̺�Ʈ
	//	wsprintf( TransChatMessage.szMessage , "��ǰ����ó: �غ�� �������� ���� �ȷ����ȳ�.. ��.��" );
		wsprintf( TransChatMessage.szMessage , "%s", StateInitEvent1 ); // �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ
		TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
		TransChatMessage.dwIP = 1;
		TransChatMessage.dwObjectSerial = 0;
		lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
		return FALSE;
	}


	for(cnt=0;cnt<rsEventUser_RecvedCount_StateInit;cnt++) { // �庰 - ��й� �̺�Ʈ
		if ( lpPlayInfo->dwCode_ID==rsEventUser_RecedList_StateInit[cnt].dwIDCode && lstrcmpi(lpPlayInfo->szID,rsEventUser_RecedList_StateInit[cnt].szID)==0 )
		{
			//�̹� �޾Ҵ�
		//	wsprintf( TransChatMessage.szMessage , "��ǰ����ó: �ڳ״� �̹� �޾Ҵٳ�~ �� ���ϰŵ� �����ۼ����� ������! " );
			wsprintf( TransChatMessage.szMessage , "%s", StateInitEvent2 ); // �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ
			TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
			TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
			TransChatMessage.dwIP = 1;
			TransChatMessage.dwObjectSerial = 0;
			lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );

			return FALSE;
		}
	}

	//������ ��Ȱ��ȭ�Ȱ�� Ȱ��ȭ ��Ŵ
	if ( !lpPlayInfo->lpPostBoxItem ) rsLoadPostBox(lpPlayInfo);
	//if ( !lpPlayInfo->lpPostBoxItem ) return FALSE;
	if ( lpPlayInfo->lpPostBoxItem && lpPlayInfo->lpPostBoxItem->ItemCounter>POST_ITEM_MAX-1 ) return FALSE;


	rsPOST_BOX_ITEM		*lpPostBox = new rsPOST_BOX_ITEM;
	if ( !lpPostBox ) return FALSE;

	ZeroMemory( lpPostBox , sizeof(rsPOST_BOX_ITEM) );

	
	lpPostBox->PostItem[0].dwItemCode = (sinBI1|sin02); // �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ
	lpPostBox->PostItem[0].Flag++;
	lstrcpy( lpPostBox->PostItem[0].szCharName , "***" );
	lstrcpy( lpPostBox->PostItem[0].szDoc , StateInitEvent3 );	// �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ
//	lstrcpy( lpPostBox->PostItem[0].szDoc , "��ų ��й� �̺�Ʈ" );
	lstrcpy( lpPostBox->PostItem[0].szFormCode , "0" );
	lstrcpy( lpPostBox->PostItem[0].szItemCode , "BI102" ); // �ű��ʵ� ������ ž 3�� ������Ʈ ��� �̺�Ʈ
	lstrcpy( lpPostBox->PostItem[0].szSpeJob , "0" );
	

	if(lpPlayInfo->lpPostBoxItem) {
		memcpy( &lpPostBox->PostItem[1] , &lpPlayInfo->lpPostBoxItem->PostItem[0] , sizeof(_POST_BOX_ITEM)*lpPlayInfo->lpPostBoxItem->ItemCounter );
		lpPostBox->ItemCounter = lpPlayInfo->lpPostBoxItem->ItemCounter+1;
		lpPostBox->dwHead = lpPlayInfo->lpPostBoxItem->dwHead;

		delete lpPlayInfo->lpPostBoxItem ;
		lpPlayInfo->lpPostBoxItem  = lpPostBox;
	}
	else {
		lpPostBox->ItemCounter = 1;
		lpPlayInfo->lpPostBoxItem  = lpPostBox;
	}


	rsEventUser_RecedList_StateInit[rsEventUser_RecvedCount_StateInit].dwIDCode = lpPlayInfo->dwCode_ID;
	lstrcpy(rsEventUser_RecedList_StateInit[rsEventUser_RecvedCount_StateInit].szID,lpPlayInfo->szID );
	rsEventUser_RecvedCount_StateInit++;


	//���� ++ ������ ����
//	wsprintf( TransChatMessage.szMessage , "��ǰ����ó: �ڳ��� �ҿ��� ����ְڳ�~ �״��� ��ų�� �����ϰ� �й��ϵ��� �ϰԳ� " );
	wsprintf( TransChatMessage.szMessage , "%s", StateInitEvent4 );
	TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
	TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
	TransChatMessage.dwIP = 1;
	TransChatMessage.dwObjectSerial = 0;
	lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );


	smTRANS_COMMAND smTransCommand;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_YAHOO_MOTION;
	lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

#endif

	return TRUE;


}

