/*----------------------------------------------------------------------------*
*		�ű� Ʃ�丮�� ����Ʈ ��������� �ֱ�			
*-----------------------------------------------------------------------------*/

/*
	switch( lpTransCommand->WParam )
	{
		//�ǰ��� â������
		case HAQUEST_CODE_ELEMENTARY_A:
			if ( lpTransCommand->LParam==1 )
			{
				rsPutItem( lpPlayInfo , sinQT1|sin15 );			//�� �ش�
			}

			if ( lpTransCommand->LParam==2 ) {					//���� ������ ���� ���� ����
				dwItemCode = 0;

				switch( lpPlayInfo->smCharInfo.JOB_CODE )
				{
				case JOBCODE_MECHANICIAN:
					dwItemCode = sinWC1|sin02;					
					break;
				case JOBCODE_FIGHTER:
					dwItemCode = sinWA1|sin02;					
					break;
				case JOBCODE_PIKEMAN:
					dwItemCode = sinWP1|sin02;					
					break;
				case JOBCODE_ARCHER:
					dwItemCode = sinWS1|sin02;
					break;
				case JOBCODE_KNIGHT:
					dwItemCode = sinWS2|sin02;				
					break;
				case JOBCODE_ATALANTA:
					dwItemCode = sinWT1|sin02;			
					break;
				case JOBCODE_PRIESTESS:
					dwItemCode = sinWM1|sin01;		
					break;
				case JOBCODE_MAGICIAN:
					dwItemCode = sinWM1|sin01;		
					break;
				case JOBCODE_ASSASSINE:
					dwItemCode = sinWD1 | sin01;
					break;
				case JOBCODE_SHAMAN:
					dwItemCode = sinWN1 | sin01;
					break;
				}
				if ( dwItemCode )
				{
					rsPutItem( lpPlayInfo , dwItemCode ,lpPlayInfo->smCharInfo.JOB_CODE);
				}
#ifdef _BEGINNER_EVENT// �ʺ�����Ʈ �̺�Ʈ�� �߰� �� ����(�ǰ��� â������)
				
				dwItemCode = 0;

				switch( lpPlayInfo->smCharInfo.JOB_CODE )
				{
				case JOBCODE_MECHANICIAN:
					dwItemCode = sinDA1|sin02;
					
					break;
				case JOBCODE_FIGHTER:
					dwItemCode = sinDA1|sin02;
					
					break;
				case JOBCODE_PIKEMAN:
					dwItemCode = sinDA1|sin02;
					
					break;
				case JOBCODE_ARCHER:
					dwItemCode = sinDA1|sin02;
					
					break;
				case JOBCODE_KNIGHT:
					dwItemCode = sinDA1|sin02;
					
					break;
				case JOBCODE_ATALANTA:
					dwItemCode = sinDA1|sin02;
					
					break;
				case JOBCODE_PRIESTESS:
					dwItemCode = sinDA2|sin02;
					
					break;
				case JOBCODE_MAGICIAN:
					dwItemCode = sinDA2|sin02;
					break;

				case JOBCODE_ASSASSINE:
					dwItemCode = sinDA2|sin02;
					break;

				case JOBCODE_SHAMAN:
					dwItemCode = sinDA2 | sin02;
					break;
				}
				if ( dwItemCode )
				{
					rsPutItem( lpPlayInfo , dwItemCode ,lpPlayInfo->smCharInfo.JOB_CODE);
				}
#endif
			}
			break;
		//�η��� ���
		case HAQUEST_CODE_ELEMENTARY_B:
			if ( lpTransCommand->LParam==2 ) {   //���� ������ �ϸ� 
				rsPutItem( lpPlayInfo ,sinOA2|sin02,lpPlayInfo->smCharInfo.JOB_CODE);

#ifdef _BEGINNER_EVENT	//�ʺ�����Ʈ �߰� ������ ���� (�η�����)
				rsPutItem( lpPlayInfo ,sinPL1|sin01,10);   //��
				rsPutItem( lpPlayInfo ,sinPS1|sin01,10);   //��
				rsPutItem( lpPlayInfo ,sinPM1|sin01,10);   //��	

				rsSendMoneyToUser(lpPlayInfo ,1000);		//��
#endif
			}
			break;
		//���� ���
		case HAQUEST_CODE_ELEMENTARY_C:
			if ( lpTransCommand->LParam==1 )  //������ �ִ�.
			{
				rsPutItem( lpPlayInfo ,sinQT1|sin14);
			}
			if ( lpTransCommand->LParam==2 )  //���� ������ ����
			{
				rsPutItem( lpPlayInfo ,sinPL1|sin01,10);   //��
				rsPutItem( lpPlayInfo ,sinPS1|sin01,10);   //��
				rsPutItem( lpPlayInfo ,sinPM1|sin01,10);   //��

#ifdef _BEGINNER_EVENT
				rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
				rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
#endif
			}
			break;
		//��� ����
		case HAQUEST_CODE_ELEMENTARY_D:
			if ( lpTransCommand->LParam==2 )  //���� ������ ��  
			{
				rsPutItem( lpPlayInfo ,sinOA1|sin06,lpPlayInfo->smCharInfo.JOB_CODE);   //�� �ƹķ�
				rsPutItem( lpPlayInfo ,sinOR1|sin06,lpPlayInfo->smCharInfo.JOB_CODE);   //����
				rsPutItem( lpPlayInfo ,sinOR1|sin06,lpPlayInfo->smCharInfo.JOB_CODE);   //����			
			}
#ifdef	_BEGINNER_EVENT
			if ( lpTransCommand->LParam==2 )
			{					//���� ������ ���� ���� ����
				dwItemCode = 0;

				switch( lpPlayInfo->smCharInfo.JOB_CODE )
				{
				case JOBCODE_MECHANICIAN:
					dwItemCode = sinWC1|sin06;
					
					break;
				case JOBCODE_FIGHTER:
					dwItemCode = sinWA1|sin06;
					
					break;
				case JOBCODE_PIKEMAN:
					dwItemCode = sinWP1|sin06;
					
					break;
				case JOBCODE_ARCHER:
					dwItemCode = sinWS1|sin06;
					
					break;
				case JOBCODE_KNIGHT:
					dwItemCode = sinWS2|sin06;
					
					break;
				case JOBCODE_ATALANTA:
					dwItemCode = sinWT1|sin06;
					
					break;
				case JOBCODE_PRIESTESS:
					dwItemCode = sinWM1|sin06;
					
					break;
				case JOBCODE_MAGICIAN:
					dwItemCode = sinWM1|sin06;
					break;
				case JOBCODE_ASSASSINE:
					dwItemCode = sinWD1|sin06;
					break;
				case JOBCODE_SHAMAN:
					dwItemCode = sinWN1|sin06;
					break;
				}
				if ( dwItemCode )
				{
					rsPutItem( lpPlayInfo , dwItemCode ,lpPlayInfo->smCharInfo.JOB_CODE);
				}
			}
#endif
			break;
		//�𵥵��� ����
		case HAQUEST_CODE_ELEMENTARY_E:
			if ( lpTransCommand->LParam==2 ) {  //���� ������ ���� 
				rsPutItem( lpPlayInfo ,sinFO1|sin04);   //����Ű ����
				rsPutItem( lpPlayInfo ,sinFO1|sin04);   //����Ű ����
#ifdef _BEGINNER_EVENT
			rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
			rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
			rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
			rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
			rsPutItem( lpPlayInfo ,sinGP1|sin17);		//���̷��� ũ����Ż
#endif
			}
			break;
		//�ܴ����� ����
		case HAQUEST_CODE_ELEMENTARY_F:
			if ( lpTransCommand->LParam==2 )  //���� ������ ����
			{
				rsPutItem( lpPlayInfo ,sinPL1|sin03,20);   //��
				rsPutItem( lpPlayInfo ,sinPS1|sin03,20);   //��
				rsPutItem( lpPlayInfo ,sinPM1|sin03,20);   //��
#ifdef _BEGINNER_EVENT
				rsPutItem( lpPlayInfo ,sinBI2|sin01);		//����� ��Ű��(3�ð�)
#endif
			}
			break;
		//�Ҿ���� ��ġ
		case HAQUEST_CODE_ELEMENTARY_G:
			if ( lpTransCommand->LParam==2 )  //���� ������ ����
			{
				rsPutItem( lpPlayInfo ,sinFO1|sin06);   //Ʈ���� ����
				rsPutItem( lpPlayInfo ,sinFO1|sin06);   //Ʈ���� ����

#ifdef	_BEGINNER_EVENT
				rsPutItem( lpPlayInfo ,sinBI1|sin13);   //����ġ���� ����(1��)
#endif
			}

#ifdef	_BEGINNER_EVENT
			if ( lpTransCommand->LParam==2 )
			{					//���� ������ ���� ���� ����
				dwItemCode = 0;

				switch( lpPlayInfo->smCharInfo.JOB_CODE )
				{
				case JOBCODE_MECHANICIAN:
					dwItemCode = sinWC1|sin11;
					
					break;
				case JOBCODE_FIGHTER:
					dwItemCode = sinWA1|sin11;
					
					break;
				case JOBCODE_PIKEMAN:
					dwItemCode = sinWP1|sin11;
					
					break;
				case JOBCODE_ARCHER:
					dwItemCode = sinWS1|sin11;
					
					break;
				case JOBCODE_KNIGHT:
					dwItemCode = sinWS2|sin11;
					
					break;
				case JOBCODE_ATALANTA:
					dwItemCode = sinWT1|sin11;
					
					break;
				case JOBCODE_PRIESTESS:
					dwItemCode = sinWM1|sin11;
					
					break;
				case JOBCODE_MAGICIAN:
					dwItemCode = sinWM1|sin11;
					break;

				case JOBCODE_ASSASSINE:
					dwItemCode = sinWD1|sin11;
					break;

				case JOBCODE_SHAMAN:
					dwItemCode = sinWN1|sin11;
					break;
				}
				if ( dwItemCode )
				{
					rsPutItem( lpPlayInfo , dwItemCode ,lpPlayInfo->smCharInfo.JOB_CODE);
				}
			}
#endif
			break;
		case HAQUEST_CODE_FURYOFPHANTOM:
			if ( lpTransCommand->LParam==2 )
			{
				switch(lpTransCommand->SParam)  //������ ���⸦ �ش�.
				{
					case 1: //����
						rsPutItemUnique(lpPlayInfo ,sinWA1|sin20,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 2: //Ŭ��
						rsPutItemUnique(lpPlayInfo ,sinWC1|sin20,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 3: //�ظ�
						rsPutItemUnique(lpPlayInfo ,sinWH1|sin21,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 4: //â
						rsPutItemUnique(lpPlayInfo ,sinWP1|sin21,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 5: //Ȱ
						rsPutItemUnique(lpPlayInfo ,sinWS1|sin22,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 6: //��
						rsPutItemUnique(lpPlayInfo ,sinWS2|sin23,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 7: //��â
						rsPutItemUnique(lpPlayInfo ,sinWT1|sin21,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 8: //�ϵ�
						rsPutItemUnique(lpPlayInfo ,sinWM1|sin21,lpPlayInfo->smCharInfo.JOB_CODE,ITEM_UNIQUE_CODE_QUEST);
					break;
					case 9: //Dagger
						rsPutItemUnique(lpPlayInfo, sinWD1 | sin21, lpPlayInfo->smCharInfo.JOB_CODE, ITEM_UNIQUE_CODE_QUEST);
					break;
					case 10: //Phantom
						rsPutItemUnique(lpPlayInfo, sinWN1 | sin21, lpPlayInfo->smCharInfo.JOB_CODE, ITEM_UNIQUE_CODE_QUEST);
					break;
						
				}
			}
			break;
	}

	//lpPlayInfo->dwHaQuestItemCount ++;
	*/