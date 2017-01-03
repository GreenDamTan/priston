#include "..\\HoBaram\\HoTextFile.h"

char *HoTextLoginMessage[20] =  {
				"正在连接....",
				"无法连接服务器",
				"帐号不存在",
				"密码错误",
				"帐号已停封,请联系精灵客服",
				"帐号已经登陆",
				"您的客户端版本太低,请先运行更新程序",
				"服务器已满员,请稍后再尝试.",
				"连接失败",
				"您的余额不足，请到http://pt.yetime.cn进行帐户储值",
				"帐户已过期",
				"正在申请删除服务器帐号...",
				"服务器超载,请稍后再尝试.",
				"请稍后再尝试 ...",
				"帐号已停封，请联系精灵客服",
				"密码中不允许使用个性字符.",
				"未满12岁的用户无法进行游戏.",
				"同意精灵玩家守则才可以进行游戏.",
	/*
		"     正在连接服务器，请稍候……   ",
		"       无法连接服务器       ",
		"     帐号错误，请重新输入     ",
		"     密码错误，请重新输入     ",
		"       您无权进入此游戏      ",
		"       这个帐号已经在线       ",
		"   您的客户端版本太低，请先运行更新程序   ",
		"     服务器已满，请稍候再试     ",
		"     与服务器的连接已中断     ",
		"您的余额不足，请到http://pt.yetime.cn进行帐户储值",
		"您的余额不足，请到http://pt.yetime.cn进行帐户储值",
		" 正在申请删除服务器帐号... ",
		"服务器超载 , 请稍后再尝试.",
		"请稍后再尝试.",
		"帐号已停封，请联系精灵客服",
		"密码中不允许使用个性字符.",
		"未满12岁的用户无法进行游戏.",
		"同意精灵玩家守则才可以进行游戏.",
		*/
		};

char *HoTextSelectMessage[12] = {
		" 您确定要删除这个角色吗? ",
		" 开始游戏 ",
		" 您的角色数目已到上限，不能再创建新角色。 ",
		" 角色设定完毕，是否确认？ ", 	//3
		" 请输入角色的名字 ", 		//4
		" 这个名字已被使用，请重新输入 ", 	//5
		" 请选择角色 ", 			//6
		" 请选择种族 ", 			//7
		" 请选择角色 ", 			//8
		" 请选择角色的造型 ", 		//9
		"无法删除会长", //10
		"无法确认工会信息" //11
		};

char *HoTextTempscronInfo[5] = {
		"坦普族：他们的祖先是古代在战场上冲锋陷阵的勇士，至今仍流传着他们的传说。他们在机械方面很发达，在商业上也很繁荣。坦普族有武士、机械兵、枪兵、弓箭手等职业，大部分非常善于战斗。他们的身手敏捷，能熟练地使用各种武器。",
		
		" 能熟练操作攻击性机械的唯一角色，懂得充分地应用机械武器的特性。但并不好战。 ",
		" 主要擅长空手或使用近距离武器攻击，在近距离作战时能发挥强大威力。肌肉发达，非常好战。 ",
		" 能熟练使用镖枪和长矛，具有宽阔的视野和出色的观察能力，在险恶的地形中也能穿梭自如。 ",
		" 擅长远距离攻击，主要使用弓箭类武器，体态轻盈、身手敏捷。 ",
		};


char *HoTextMoryon = "魔灵族";
char *HoTextMoryonInfo[5] = {
	"《精灵》的第二种族，也就是偏重于魔法系统的魔灵族（Morayion），居住在精灵大陆的北部，信仰着名叫Pritos的神。他们以坚定的信念，诚实的性格，顽强地生活在精灵大陆。虽然他们是精灵大陆中最为温顺的种族，但是魔灵族最发达的精神文明改造了他们的战斗力。也许魔灵族中的战士也展现出他们柔弱的一面，具有比之其他种族更为敏捷的体型，但是他们并非以肌肉来展现强大，而完全依靠着自己的精神能力。魔灵族比之于其他种族，总的来说给人一种健康向上的感觉。", 
   " 游侠（Knight）: 他们拥有神圣的力量，而在使用剑的时候能发挥出他们最强大的力量。从他的身上既可以领略到威猛战士的威严，而更重要的是他的魔力将完全结合在其战斗力中。", 
 
    "魔枪手（Atalanta）: 这是一个以敏捷擅长的职业，她们的名字正是西腊神话中著名的赤足神女的名字，也许她们的严肃表情与魔灵族温顺的性格不太符合，但是同样她们具有强大的魔法。她们最擅长的武器是标枪，传说中她们是坦普族长枪兵职业的后代，但确切的起源谁也不知道。虽然从造型上感觉她们与魔灵族有所区别，但是从她们华丽的服装以及敏捷的身法上，还是能体现魔灵族的特色。", 
 
    "祭司（Priestess）: 她们具有善良的心，对于神圣的信仰她们将用自己的生命去维护。助人为乐是她们的性格，修长的身材展现出魔灵族女人的特色，她们的擅长武器是具有魔法的杖。", 
 
    "魔法师（Magician）: 他们具有强大的精神力，并且可以发挥大陆上最巨大的魔法。这正是典型的魔法师，而在他们的手上，魔杖将发挥出更大的威力。比起祭司来，他们手中的魔杖更为可怕。",

};

char *HoText_PartyMessage = "申请组队";

char *HoTextTempskron = "坦普族";

char *HoTextFriendNear = "[最近列表]";
char *HoTextFriendList = "[朋友列表] %d/%d";
char *HoTextFriendRefuse = "[禁言列表] %d/%d";



 