#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#define L_User sizeof(struct user)
#define L_Price sizeof(struct price)
#define L_List sizeof(struct list)
typedef struct user//用户信息
{
	char identity_number[20];//身份证号
	char name[20];//姓名
	char password[20];//密码
	int type;//0为普通用户,1为工作人员，2为管理员
	struct user* next;
}User;
typedef struct price//物品完税价格
{
	int number;//税号
	char name[30];//物品名称
	char unit[20];//单位
	float price;//完税价格
	int tariff;//税率百分率
	struct price* upper;//父结点所在链表头指针，用于输出菜单
	struct price* lower;//子结点所在链表头指针，用于输出菜单
	struct price* next;//下一结点
}Price;
typedef struct list//申报列表
{
	long number;//申报号，数据为1970年1月1日0:00到系统时间的秒数
	int price_number;//税号
	char name[30];//物品名称
	char unit[20];//单位
	float quantity;//数量
	float price;//税费
	char user_number[20];//用户身份证号
	char user_name[20];//用户姓名
	int status;//申报状态，0为待受理，1为通过，2为退回，3为销毁
	struct list* next;//下一结点
}List;
User* read_user();
User* sign_in(int i, User* head);
User* sign_up(int n, User* head);
User* user_set_up(User* p, User* head);
User* logout(char num[], User* head);
User* del_user(User* head);
Price* read_price();
Price* goods_choose(Price* head);
List* read_list();
List* goods_add(User* up, Price* pp, List* head);
void get_password(char pw[]);
void user_fprintf(User* head);
void goods_menu(Price* head);
void goods_report(User* up, Price* p_head, List* l_head);
void goods_printf(User* up, List* l_head);
void goods_set_up(User* up, List* l_head);
void goods_fprint(List* head);
void type_user(User* up, User* u_head, Price* p_head, List* l_head);
void type_staff(User* up, User* u_head, Price* p_head, List* l_head);
void type_admin(User* up, User* u_head, Price* p_head, List* l_head);
char* str_rand(int n);
int main()
{
	int i = 1;
	User* u_head, * user;
	Price* p_head;
	List* l_head;
	u_head = read_user();
	p_head = read_price();
	l_head = read_list();
	while (i != 0)
	{
		system("cls");
		printf("\t\t欢迎使用海关个人邮寄物品的申报及验放系统，请登录或注册。\n");
		printf("\t\t********************************************************\n");
		printf("\t\t*                       1.登  录                       *\n");
		printf("\t\t*                       2.注  册                       *\n");
		printf("\t\t*                   3.退  出  系  统                   *\n");
		printf("\t\t********************************************************\n");
		printf("\t\t请选择功能：");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			user = sign_in(0, u_head);
			switch (user->type)
			{
			case 0:
				type_user(user, u_head, p_head, l_head);
				break;
			case 1:
				type_staff(user, u_head, p_head, l_head);
				break;
			case 2:
				type_admin(user, u_head, p_head, l_head);
				break;
			}
			break;
		case 2:
			u_head = sign_up(0, u_head);
			break;
		case 0:
			break;
		default:
			printf("输入错误，请重新输入。");
			break;
		}
	}
	return 0;
}
User* read_user()//读取用户文件，返回头结点
{
	User* p, * head = NULL, * rear = NULL;
	FILE* fp;
	while ((fp = fopen("user_list", "rb")) == NULL)
	{
		printf("用户列表初始化失败，请设置管理员账号。");
		system("pause");
		sign_up(2, head);
	}
	while (!feof(fp))
	{
		p = (User*)malloc(L_User);
		fread(p, L_User, 1, fp);
		if (head == NULL)
			head = p;
		else
			rear->next = p;
		rear = p;
	}
	rear->next = NULL;
	fclose(fp);
	return head;
}
User* sign_in(int i, User* head)//登录，参数（登录次数，用户头结点），返回登录用户结点
{
	User* p;
	char identity_number[20], password[20], * str, str1[5];
	system("cls");
	p = head;
	printf("\t\t**************************登  录**************************\n");
	printf("\t\t身份证号：");
	gets(identity_number);
	printf("\t\t密码：");
	get_password(password);
	if (i >= 3)
	{
		str = str_rand(4);
		printf("\n\t\t验证码：%s\n", str);
		printf("\t\t请输入验证码：");
		gets(str1);
		if ((strcmp(str, str1)) != 0)
		{
			printf("\t\t验证码错误，请重新登录。\n");
			system("pause");
			return sign_in(i + 1, head);
		}
	}
	while (p != NULL)
	{
		if ((strcmp(p->identity_number, identity_number)) == 0)
		{
			if ((strcmp(p->password, password)) == 0)
			{
				printf("登录成功！\n");
				system("pause");
				return p;
			}
			else
			{
				printf("密码错误，请重新登录。\n");
				system("pause");
				break;
			}
		}
		p = p->next;
	}
	if (p == NULL)
	{
		printf("身份证号错误，按“y”注册，按任意键重新登录。\n");
		if (getchar() == 'y')
			head = sign_up(0, head);
	}
	return sign_in(i + 1, head);
}
User* sign_up(int n, User* head)//注册，参数（用户类型，用户头结点），返回头结点
{
	User* p, * p1;
	FILE* fp;
	fp = fopen("user_list", "ab");
	p = head;
	p1 = (User*)malloc(L_User);
	system("cls");
	printf("\t\t**************************注  册**************************\n");
	printf("\t\t身份证号：");
	gets(p1->identity_number);
	if (strlen(p1->identity_number) != 18)
	{
		printf("\t\t身份证号长度不合法，请重新输入。");
		system("pause");
		return sign_up(n, head);
	}
	printf("\t\t姓名：");
	gets(p1->name);
	printf("\t\t密码：");
	get_password(p1->password);
	p1->type = n;
	p1->next = NULL;
	if (head != NULL)
	{
		while (p->next != NULL)
			p = p->next;
		p->next = p1;
	}
	else
		head = p1;
	fwrite(p1, L_User, 1, fp);
	printf("\t\t注册成功！\n");
	system("pause");
	fclose(fp);
	return head;
}
User* user_set_up(User* p, User* head)//个人信息查看及修改，参数（当前用户结点，用户头结点），返回当前用户结点
{
	system("cls");
	printf("当前用户信息：");
	printf("姓名：%s\n", p->name);
	printf("身份证号：%s\n", p->identity_number);
	printf("用户类型：\n");
	switch (p->type)
	{
	case 0:
		printf("普通用户\n");
		break;
	case 1:
		printf("工作人员\n");
		break;
	case 2:
		printf("管理员\n");
		break;
	}
	printf("1.修改个人信息  2.注销账户  按任意键返回\n");
	switch (getchar())
	{
	case '1':
		getchar();
		printf("身份证号：");
		gets(p->identity_number);
		printf("姓名：");
		gets(p->name);
		printf("密码：");
		get_password(p->password);
		printf("修改成功！");
		system("pause");
		break;
	case '2':
		head = logout(p->identity_number, head);
		return NULL;
	default:
		break;
	}
	return p;
}
User* logout(char num[], User* head)//注销账户，参数（注销账户身份证号，用户头结点），返回用户头结点
{
	User* p, * p1;
	p = head;
	while ((strcmp(num, p->identity_number)) != 0)
	{
		p1 = p;
		p = p->next;
	}
	if (p == head)
		head = p->next;
	else
		p1->next = p->next;
	free(p);
	user_fprintf(head);
	return head;
}
User* del_user(User* head)//删除用户，参数（用户头结点），返回用户头结点
{
	char num[20];
	User* p;
	p = head;
	system("cls");
	while (p != NULL)
	{
		if (p->type == 1)
			printf("%s %s\n", p->identity_number, p->name);
		p = p->next;
	}
	printf("请输入需要删除用户的身份证号：");
	gets(num);
	head = logout(num, head);
	return head;
}
Price* read_price()//读取完税价格文件，返回完税价格头结点
{
	Price* p, * head = NULL, * p1 = NULL, * p2 = NULL, * p3 = NULL, * p4 = NULL, * p5 = NULL;
	FILE* fp;
	if ((fp = fopen("price_list.txt", "r")) == NULL)
	{
		printf("读取物品完税价格失败，程序退出。");
		exit(0);
	}
	while (!feof(fp))
	{
		p = (Price*)malloc(L_Price);
		p->lower = NULL;
		if ((fscanf(fp, "%d%s%s%f%d", &p->number, p->name, p->unit, &p->price, &p->tariff)) == -1)
		{
			free(p);
			break;
		}
		if ((p->number / 10000) % 10 == 0)//判断是否为一级菜单
		{
			if (head == NULL)
				head = p;
			else
				p1->next = p;
			p->upper = NULL;
			p1 = p;
			if (p2 != NULL)
				p2->next = NULL;
			p2 = NULL;
		}
		else
		{
			if ((p->number / 100) % 10 == 0)//判断是否为二级菜单
			{
				if (p2 == NULL)
				{
					p2 = p;
					p1->lower = p2;
				}
				else
					p2->next = p;
				p->upper = head;
				p2 = p;
				if (p3 != NULL)
					p3->next = NULL;
				p3 = NULL;
			}
			else
			{
				if ((p->number / 10) % 10 == 0)//判断是否为三级菜单
				{
					if (p3 == NULL)
					{
						p3 = p;
						p2->lower = p3;
					}
					else
						p3->next = p;
					p->upper = p1->lower;
					p3 = p;
					if (p4 != NULL)
						p4->next = NULL;
					p4 = NULL;
				}
				else
				{
					if (p->number % 10 == 0)//判断是否为四级菜单
					{
						if (p4 == NULL)
						{
							p4 = p;
							p3->lower = p4;
						}
						else
							p4->next = p;
						p->upper = p2->lower;
						p4 = p;
						if (p5 != NULL)
							p5->next = NULL;
						p5 = NULL;
					}
					else
					{
						if (p5 == NULL)
						{
							p5 = p;
							p4->lower = p5;
						}
						else
							p5->next = p;
						p->upper = p3->lower;
						p5 = p;
					}
				}
			}
		}
	}
	p1->next = NULL;
	fclose(fp);
	return head;
}
Price* goods_choose(Price* head)//申报物品选择，参数（完税价格头结点），返回申报物品头结点
{
	Price* p;
	p = head;
	int num;
	goods_menu(p);
	printf("\n请输入所选择税号：（输入0返回上一级菜单）");
	scanf("%d", &num);
	if (num == 0)
		if (p->upper == NULL)
		{
			printf("已是最高级菜单。\n");
			return;
		}
		else
			return goods_choose(p->upper);
	while (p != NULL)
	{
		if (num == p->number)
			break;
		else
			p = p->next;
	}
	if (p == NULL)
	{
		printf("税号输入错误，请重新输入。\n");
		system("pause");
	}
	else
		if (p->lower == NULL)
			return p;
		else
			return goods_choose(p->lower);
	return goods_choose(head);
}
List* read_list()//读取申报列表文件，返回申报列表头结点
{
	List* p, * head = NULL, * rear;
	FILE* fp;
	while ((fp = fopen("goods_list.txt", "r")) == NULL)
	{
		printf("打开申报列表文件失败，请添加后重新打开程序。");
		exit(0);
	}
	while (!feof(fp))
	{
		p = (List*)malloc(L_List);
		if ((fscanf(fp, "%ld%d%s%s%f%f%s%s%d", &p->number, &p->price_number, p->name, p->unit, &p->quantity, &p->price, p->user_number, p->name, &p->status)) == -1)
		{
			free(p);
			break;
		}
		if (head == NULL)
			head = p;
		else
			rear->next = p;
		rear = p;
	}
	if (head != NULL)
		rear->next = NULL;
	fclose(fp);
	return head;
}
List* goods_add(User* up, Price* pp, List* l_head)//添加申报列表项目,参数（用户结点，完税价格头结点，申报列表头结点），返回申报列表头结点
{
	List* p, * p1;
	FILE* fp;
	if ((fp = fopen("goods_list.txt", "a")) == NULL)
	{
		printf("打开申报列表文件失败，请添加后重新打开程序。");
		exit(0);
	}
	p = (List*)malloc(L_List);
	printf("请输入数量：(%s)", pp->unit);
	scanf("%f", &p->quantity);
	if (pp->price == 0)
	{
		printf("完税价格需另行确认，请输入零售价：");
		scanf("%f", &p->price);
		p->price = p->quantity * p->price;
	}
	else
		p->price = p->quantity * pp->price;
	p->number = (long)time(NULL);
	p->status = 0;
	p->price_number = pp->number;
	strcpy(p->name, pp->name);
	strcpy(p->unit, pp->unit);
	strcpy(p->user_name, up->name);
	strcpy(p->user_number, up->identity_number);
	if (l_head == NULL)
		l_head = p;
	else
	{
		p1 = l_head;
		while (p1->next != NULL)
			p1 = p1->next;
		p1->next = p;
	}
	p->next = NULL;
	fprintf(fp, "%ld %d %s %s %f %f %s %s %d\n", p->number, p->price_number, p->name, p->unit, p->quantity, p->price, p->user_number, p->name, p->status);
	fclose(fp);
	return l_head;
}
void get_password(char pw[])//密码输入，参数（待输入密码）
{
	int i = 0;
	while ((pw[i] = getch()) != '\r')
	{
		if (pw[i] == '\b')//此处'\b'指退格键
		{
			if (i != 0)
			{
				printf("\b \b");
				i--;
			}
		}
		else
		{
			printf("*");
			i++;
		}
	}
	pw[i] = '\0';
}
void user_fprintf(User* head)//输出用户列表到文件，参数（用户头结点）
{
	User* p;
	FILE* fp;
	p = head;
	if ((fp = fopen("user_list1", "wb")) == NULL)
	{
		printf("打开用户列表文件失败，请添加后重新打开程序。");
		exit(0);
	}
	while (p != NULL)
	{
		fwrite(p, L_User, 1, fp);
		p = p->next;
	}
	fclose(fp);
	remove("user_list");
	rename("user_list1", "user_list");
}
void goods_menu(Price* head)//物品菜单，（物品头结点）
{
	int i = 0;
	Price* p;
	p = head;
	system("cls");
	while (p != NULL)
	{
		printf("%8d:%-30s", p->number, p->name);
		p = p->next;
		i++;
		if (i % 2 == 0)
			printf("\n");
	}
}
void goods_report(User* up, Price* p_head, List* l_head)//物品申报，参数（用户结点，完税价格头结点，申报列表头结点）
{
	Price* pp;
	pp = goods_choose(p_head);
	l_head = goods_add(up, pp, l_head);
	printf("申报成功。");
	system("pause");
}
void goods_printf(User* up, List* l_head)//申报物品输出，参数（用户结点，申报列表头结点）
{
	List* lp;
	lp = l_head;
	system("cls");
	while (lp != NULL)
	{
		if ((strcmp(lp->user_number, up->identity_number)) == 0 || up->type == 1)
		{
			printf("申报号：%ld\n税号：%d\n物品名称：%s\n数量：%.2f(%s)\n税费：%.2f\n", lp->number, lp->price_number, lp->name, lp->quantity, lp->unit, lp->price);
			switch (lp->status)
			{
			case 0:
				printf("状态：待受理\n\n");
				break;
			case 1:
				printf("状态：通过\n\n");
				break;
			case 2:
				printf("状态：退回\n\n");
				break;
			case 3:
				printf("状态：销毁\n\n");
				break;
			}
		}
		lp = lp->next;
	}
}
void goods_set_up(User* up, List* l_head)//处理申报项目，用户进行删除，工作人员进行状态修改，参数（用户结点，申报列表头结点）
{
	long num;
	List* lp, * lp1;
	for (;;)
	{
		lp = l_head;
		printf("请输入需要处理的申报号：");
		scanf("%ld", &num);
		getchar();
		while (lp != NULL)
		{
			if (num == lp->number)
				break;
			lp1 = lp;
			lp = lp->next;
		}
		if (lp == NULL)
			printf("申报号不存在，请重新输入。");
		else
			break;
	}
	switch (up->type)
	{
	case 0:
		if (strcmp(lp->user_number, up->identity_number) != 0)
		{
			printf("所选申报号非本人申报，请重新选择。");
			break;
		}
		else
		{
			printf("按y确认删除，按任意键取消。");
			if (getchar() == 'y')
			{
				if (lp == l_head)
					l_head = lp->next;
				else
					lp1->next = lp->next;
				free(lp);
			}
			break;
		}
	case 1:
		printf("请选择受理结果：0.暂不处理  1.通过  2.退回  3.销毁\n");
		scanf("%d", &num);
		while (num < 0 || num>3)
		{
			printf("输入不合法，请重新输入。");
			scanf("%d", &num);
		}
		lp->status = num;
		break;
	}
	goods_fprint(l_head);
}
void goods_fprint(List* head)//输出申报列表到文件，参数（申报列表头结点）
{
	List* p;
	FILE* fp;
	p = head;
	if ((fp = fopen("goods_list.txt", "w")) == NULL)
	{
		printf("打开申报列表文件失败，请添加后重新打开程序。");
		exit(0);
	}
	while (p != NULL)
	{
		fprintf(fp, "%ld %d %s %s %f %f %s %s %d\n", p->number, p->price_number, p->name, p->unit, p->quantity, p->price, p->user_number, p->name, p->status);
		p = p->next;
	}
	fclose(fp);
}
void type_user(User* up, User* u_head, Price* p_head, List* l_head)//用户菜单
{
	int i = 1;
	while (i != 0)
	{
		system("cls");
		printf("\t\t%s，欢迎使用海关个人邮寄物品申报\n", up->name);
		printf("\t\t********************************************************\n");
		printf("\t\t*                     1.邮递物品申报                   *\n");
		printf("\t\t*                     2.申报状态查询                   *\n");
		printf("\t\t*                     3.个人信息查询                   *\n");
		printf("\t\t*                     0. 退 出 登 录                   *\n");
		printf("\t\t********************************************************\n");
		printf("\t\t请选择功能：");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			goods_report(up, p_head, l_head);
			break;
		case 2:
			goods_printf(up, l_head);
			printf("按y删除申报，按任意键返回。");
			if (getchar() == 'y')
				goods_set_up(up, l_head);
			break;
		case 3:
			up = user_set_up(up, u_head);
			if (up == NULL)
				return;
			break;
		case 0:
			return;
		default:
			printf("输入错误，请重新选择。");
			system("pause");
			break;
		}
	}
}
void type_staff(User* up, User* u_head, Price* p_head, List* l_head)//工作人员菜单
{
	int i = 1;
	while (i != 0)
	{
		system("cls");
		printf("\t\t%s，欢迎使用海关个人邮寄物品申报\n", up->name);
		printf("\t\t********************************************************\n");
		printf("\t\t*                     1. 申 报 审 批                   *\n");
		printf("\t\t*                     2.申报列表查询                   *\n");
		printf("\t\t*                     3.个人信息查询                   *\n");
		printf("\t\t*                     0. 退 出 登 录                   *\n");
		printf("\t\t********************************************************\n");
		printf("\t\t请选择功能：");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			goods_printf(up, l_head);
			goods_set_up(up, l_head);
			system("pause");
			break;
		case 2:
			goods_printf(up, l_head);
			system("pause");
			break;
		case 3:
			up = user_set_up(up, u_head);
			if (up == NULL)
				return;
			break;
		case 0:
			return;
		default:
			printf("输入错误，请重新选择。");
			system("pause");
			break;
		}
	}
}
void type_admin(User* up, User* u_head, Price* p_head, List* l_head)//管理员菜单
{
	int i = 1;
	while (i != 0)
	{
		system("cls");
		printf("\t\t%s，欢迎使用海关个人邮寄物品申报\n", up->name);
		printf("\t\t********************************************************\n");
		printf("\t\t*                     1.注册工作人员                   *\n");
		printf("\t\t*                     2.删除工作人员                   *\n");
		printf("\t\t*                     3.个人信息查询                   *\n");
		printf("\t\t*                     0. 退 出 登 录                   *\n");
		printf("\t\t********************************************************\n");
		printf("\t\t请选择功能：");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			u_head = sign_up(1, u_head);
			break;
		case 2:
			u_head = del_user(u_head);
			break;
		case 3:
			up = user_set_up(up, u_head);
			if (up == NULL)
				return;
			break;
		case 0:
			return;
		default:
			printf("输入错误，请重新选择。");
			system("pause");
			break;
		}
	}
}
char* str_rand(int n)//生成随机验证码，参数为验证码位数
{
	int flag, i;
	char* str;
	srand((unsigned)time(NULL));
	str = (char*)malloc((n + 1) * sizeof(char));
	for (i = 0; i < n; i++)
	{
		flag = rand() % 2;
		switch (flag)
		{
		case 0:
			str[i] = 'A' + rand() % 26;
			break;
		case 1:
			str[i] = 'a' + rand() % 26;
			break;
		}
	}
	str[n] = '\0';
	return str;
}
