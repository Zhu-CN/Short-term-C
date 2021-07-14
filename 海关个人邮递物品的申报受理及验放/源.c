#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#define L_User sizeof(struct user)
#define L_Price sizeof(struct price)
#define L_List sizeof(struct list)
typedef struct user//�û���Ϣ
{
	char identity_number[20];//���֤��
	char name[20];
	char password[20];
	int type;//0Ϊ��ͨ�û�,1Ϊ������Ա��2Ϊ����Ա
	struct User* next;
}User;
typedef struct price//��Ʒ��˰�۸�
{
	int number;//˰��
	char name[30];
	char unit[20];//��λ
	float price;//��˰�۸�
	int tariff;//˰�ʰٷ���
	struct price* upper;//�������������ͷָ��
	struct price* lower;//�ӽ����������ͷָ��
	struct price* next;
}Price;
typedef struct list//�걨�б�
{
	long number;//�걨��
	int price_number;//˰��
	char name[30];//��Ʒ����
	char unit[20];//��λ
	float quantity;//����
	float price;//˰��
	char user_number[20];//�û����֤��
	char user_name[20];//�û�����
	int status;//�걨״̬��0Ϊ������1Ϊͨ����2Ϊ�˻أ�3Ϊ����
	struct list* next;
}List;
User* read_user();
User* sign_in(User* head);
User* sign_up(int n, User* head);
User* user_set_up(User* p, User* head);
User* logout(char num[], User* head);
User* del_user(User* head);
Price* read_price();
Price* search_price(int num, Price* head);
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
int main()
{
	int i;
	User* u_head, * user;
	Price* p_head;
	List* l_head;
	u_head = read_user();
	p_head = read_price();
	l_head = read_list();
	for (;;)
	{
		system("cls");
		printf("��ӭʹ�ú��ظ����ʼ���Ʒ���걨�����ϵͳ�����¼��ע�ᡣ\n");
		printf("1.��¼  2.ע��\n��ѡ��");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			user = sign_in(u_head);
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
		case 3:
			exit(0);
		default:
			printf("����������������롣");
			break;
		}
	}
	return 0;
}
User* read_user()//���û��б��ȡ���������в�����ͷָ��
{
	User* p, * head = NULL, * rear;
	FILE* fp;
	while ((fp = fopen("user_list", "rb")) == NULL)
	{
		printf("�û��б��ʼ��ʧ�ܣ������ù���Ա�˺š�");
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
	return head;
}
User* sign_in(User* head)//��¼�������û����ָ��
{
	User* p;
	char identity_number[20], password[20], c;
	while (1)
	{
		p = head;
		printf("���֤�ţ�");
		gets(identity_number);
		printf("���룺");
		get_password(password);
		while (p != NULL)
		{
			if ((strcmp(p->identity_number, identity_number)) == 0)
			{
				if ((strcmp(p->password, password)) == 0)
				{
					printf("��¼�ɹ���");
					return p;
				}
				else
				{
					printf("������������µ�¼��");
					break;
				}
			}
			p = p->next;
		}
		if (p == NULL)
		{
			printf("���֤�Ŵ��󣬰���y��ע�ᣬ����������µ�¼��\n");
			c = getchar();
			if (c == 'y')
				head = sign_up(0, head);
		}
	}
}
User* sign_up(int n, User* head)//ע�ᣬ����ͷָ��
{
	User* p, * p1;
	FILE* fp;
	fp = fopen("user_list", "ab");
	p = head;
	p1 = (struct User*)malloc(L_User);
	if (head != NULL)
	{
		while (p->next != NULL)
			p = p->next;
		p->next = p1;
	}
	else
		head = p1;
	printf("���֤�ţ�");
	gets(p1->identity_number);
	printf("������");
	gets(p1->name);
	printf("���룺");
	get_password(p1->password);
	p1->type = n;
	p1->next = NULL;
	fwrite(p1, L_User, 1, fp);
	printf("ע��ɹ���");
	fclose(fp);
	return(head);
}
User* user_set_up(User* p, User* head)//����Ϊ��ǰ�û����ָ�룬����ͬ��
{
	printf("��ǰ�û���Ϣ��");
	printf("������%s\n", p->name);
	printf("���֤�ţ�%s\n", p->identity_number);
	printf("�û����ͣ�\n");
	switch (p->type)
	{
	case 0:
		printf("��ͨ�û�\n");
		break;
	case 1:
		printf("������Ա\n");
		break;
	case 2:
		printf("����Ա\n");
		break;
	}
	printf("1.�޸ĸ�����Ϣ  2.ע���˻�  �����������\n");
	switch ((getchar()))
	{
	case '1':
		printf("���֤�ţ�");
		gets(p->identity_number);
		printf("������");
		gets(p->name);
		printf("���룺");
		gets(p->password);
		printf("�޸ĳɹ���");
		break;
	case '2':
		head = logout(p->identity_number, head);
		return NULL;
	default:
		break;
	}
	return p;
}
User* logout(char num[], User* head)//ע���˻�������ͷָ��
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
User* del_user(int n, User* head)//����û��б�ѡ�񷵻�ͷָ��
{
	char num[20];
	User* p;
	p = head;
	while (p != NULL)
	{
		if (p->type == n)
			printf("%d %s\n", p->identity_number, p->name);
		p = p->next;
	}
	printf("��������Ҫɾ���û������֤�ţ�");
	gets(num);
	p = head;
	head = logout(num, head);
	return head;
}
Price* read_price()//����Ʒ��˰�۸��ȡ�������в�����ͷָ��
{
	Price* p, * head = NULL, * p1 = NULL, * p2 = NULL, * p3 = NULL, * p4 = NULL, * p5 = NULL;
	FILE* fp;
	if ((fp = fopen("price_list.txt", "r")) == NULL)
	{
		printf("��ȡ��Ʒ��˰�۸�ʧ�ܣ������˳���");
		exit(0);
	}
	while (!feof(fp))
	{
		p = (struct Price*)malloc(L_Price);
		p->lower = NULL;
		fscanf(fp, "%d%s%s%f%d", &p->number, p->name, p->unit, &p->price, &p->tariff);
		if ((p->number / 10000) % 10 == 0)//�ж��Ƿ�Ϊһ���˵�
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
			if ((p->number / 100) % 10 == 0)//�ж��Ƿ�Ϊ�����˵�
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
				if ((p->number / 10) % 10 == 0)//�ж��Ƿ�Ϊ�����˵�
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
					if (p->number % 10 == 0)//�ж��Ƿ�Ϊ�ļ��˵�
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
	return head;
}
Price* search_price(int num, Price* head)//������������˰�����ڽ��
{
	Price* p;
	p = head;
	while (p != NULL)
	{
		if (num = p->number)
			return p;
		p = p->next;
	}
	return NULL;
}
Price* goods_choose(Price* head)//�걨��Ʒ������ָ��
{
	Price* p;
	p = head;
	int num;
	goods_menu(p);
	printf("��������ѡ��˰�ţ�������0������һ���˵���");
	scanf("%d", &num);
	while (num != p->number)
		p = p->next;
	if (num == 0)
		if (p->upper == NULL)
			printf("������߼��˵���\n");
		else
			return(goods_choose(p->upper));
	else
		if ((search_price(num, head)) == NULL)
			printf("˰������������������롣\n");
		else
			if (p->lower == NULL)
				return p;
			else
				return(goods_choose(p->lower));
	return(goods_choose(head));
}
List* read_list()//���걨�б��ȡ���������в�����ͷָ��
{
	List* p, * head = NULL, * rear;
	FILE* fp;
	while ((fp = fopen("goods_list.txt", "r")) == NULL)
	{
		printf("���걨�б��ļ�ʧ�ܣ�����Ӻ����´򿪳���");
		exit(0);
	}
	while (!feof(fp))
	{
		p = (List*)malloc(L_List);
		if ((fscanf(fp, "%ld%d%s%s%f%f%s%s", &p->number, &p->price_number, p->name, p->unit, &p->quantity, &p->price, p->user_number, p->name)) == -1)
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
	return head;
}
List* goods_add(User* up, Price* pp, List* head)//����걨�б���Ŀ,����ͷָ��
{
	List* p, * p1;
	FILE* fp;
	if ((fp = fopen("goods_list.txt", "a")) == NULL)
	{
		printf("���걨�б��ļ�ʧ�ܣ�����Ӻ����´򿪳���");
		exit(0);
	}
	p = (struct List*)malloc(L_List);
	printf("������������(%s)", pp->unit);
	scanf("%f", &p->quantity);
	if (pp->price == 0)
	{
		printf("��˰�۸�������ȷ�ϣ����������ۼۣ�");
		scanf("%f", &p->price);
		p->price = p->quantity * p->price;
	}
	else
		p->price = p->quantity * pp->price;
	p->number = time(NULL);
	p->status = 0;
	p->price_number = pp->number;
	strcpy(p->name, pp->name);
	strcpy(p->unit, pp->unit);
	strcpy(p->user_name, up->name);
	strcpy(p->user_number, up->identity_number);
	if (head == NULL)
		head = p;
	else
	{
		p1 = head;
		while (p1->next != NULL)
			p1 = p1->next;
		p1->next = p;
	}
	p->next = NULL;
	fprintf(fp, "%ld %d %s %s %f %f %s %s\n", p->number, p->price_number, p->name, p->unit, p->quantity, p->price, p->user_number, p->name);
	fclose(fp);
	return(head);
}
void get_password(char pw[])//��������
{
	int i = 0;
	while ((pw[i] = getch()) != '\r')
	{
		if (pw[i] == '\b')//�˴�'\b'ָ�˸��
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
void user_fprintf(User* head)//����û��б��ļ�
{
	User* p;
	FILE* fp;
	p = head;
	remove("user_list");
	if ((fp = fopen("user_list", "wb")) == NULL)
	{
		printf("���û��б��ļ�ʧ�ܣ�����Ӻ����´򿪳���");
		exit(0);
	}
	while (p != NULL)
	{
		fwrite(p, L_User, 1, fp);
		p = p->next;
	}
	fclose(fp);
}
void goods_menu(Price* head)//��Ʒ�˵�
{
	Price* p;
	p = head;
	while (p != NULL)
	{
		printf("%d %s\n", p->number, p->name);
		p = p->next;
	}
}
void goods_report(User* up, Price* p_head, List* l_head)//��Ʒ�걨
{
	Price* pp;
	pp = goods_choose(p_head);
	l_head = goods_add(up, pp, l_head);
	printf("�걨�ɹ���");
}
void goods_printf(User* up, List* l_head)//�걨��Ʒ���
{
	List* lp;
	lp = l_head;
	while (lp != NULL)
	{
		if ((strcmp(lp->user_number, up->identity_number)) == 0 || up->type == 1)
		{
			printf("�걨�ţ�%ld\n˰�ţ�%d\n��Ʒ���ƣ�%s\n������%.2f(%s)\n˰�ѣ�%.2f\n", lp->number, lp->price_number, lp->name, lp->quantity, lp->unit, lp->price);
			switch (lp->status)
			{
			case 0:
				printf("״̬��������\n\n");
				break;
			case 1:
				printf("״̬��ͨ��\n\n");
				break;
			case 2:
				printf("״̬���˻�\n\n");
				break;
			case 3:
				printf("״̬������\n\n");
				break;
			}
		}
		lp = lp->next;
	}
}
void goods_set_up(User* up, List* l_head)//�����걨��Ŀ���û�����ɾ����������Ա����״̬�޸�
{
	long num;
	List* lp, * lp1;
	for (;;)
	{
		lp = l_head;
		printf("��������Ҫ������걨�ţ�");
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
			printf("�걨�Ų����ڣ����������롣");
		else
			break;
	}
	switch (up->type)
	{
	case 0:
		if (strcmp(lp->user_number, up->identity_number) != 0)
		{
			printf("��ѡ�걨�ŷǱ����걨��������ѡ��");
			break;
		}
		else
		{
			printf("��yȷ��ɾ�����������ȡ����");
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
		printf("��ѡ����������0.�ݲ�����  1.ͨ��  2.�˻�  3.����\n");
		scanf("%d", &num);
		while (num < 0 || num>3)
		{
			printf("���벻�Ϸ������������롣");
			scanf("%d", &num);
		}
		lp->status = num;
		break;
	}
	goods_fprint(l_head);
}
void goods_fprint(List* head)//����걨�б��ļ�
{
	List* p;
	FILE* fp;
	p = head;
	if ((fp = fopen("goods_list.txt", "w")) == NULL)
	{
		printf("���걨�б��ļ�ʧ�ܣ�����Ӻ����´򿪳���");
		exit(0);
	}
	while (p != NULL)
	{
		fprintf(fp, "%ld %d %s %s %f %f %s %s\n", p->number, p->price_number, p->name, p->unit, p->quantity, p->price, p->user_number, p->name);
		p = p->next;
	}
	fclose(fp);
}
void type_user(User* up, User* u_head, Price* p_head, List* l_head)
{
	int i = 1;
	while (i != 0)
	{
		system("cls");
		printf("%s����ӭʹ�ú��ظ����ʼ���Ʒ�걨\n", up->name);
		printf("1.�ʵ���Ʒ�걨\n");
		printf("2.�걨״̬��ѯ\n");
		printf("3.������Ϣ��ѯ\n");
		printf("0.�˳���¼\n");
		printf("��ѡ���ܣ�");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			goods_report(up, p_head, l_head);
			break;
		case 2:
			goods_printf(up, l_head);
			printf("��yɾ���걨������������ء�");
			if ((getchar()) == 'y')
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
			printf("�������������ѡ��");
			break;
		}
	}
}
void type_staff(User* up, User* u_head, Price* p_head, List* l_head)
{
	int i = 1;
	while (i != 0)
	{
		system("cls");
		printf("%s����ӭʹ�ú��ظ����ʼ���Ʒ�걨\n", up->name);
		printf("1.�걨����\n");
		printf("2.�걨�б��ѯ\n");
		printf("3.������Ϣ��ѯ\n");
		printf("0.�˳���¼\n");
		printf("��ѡ���ܣ�");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			goods_printf(up, l_head);
			goods_set_up(up, l_head);
			break;
		case 2:
			goods_printf(up, l_head);
			break;
		case 3:
			up = user_set_up(up, u_head);
			if (up == NULL)
				return;
			break;
		case 0:
			return;
		default:
			printf("�������������ѡ��");
			break;
		}
	}
}
void type_admin(User* up, User* u_head, Price* p_head, List* l_head)
{
	int i = 1;
	while (i != 0)
	{
		system("cls");
		printf("%s����ӭʹ�ú��ظ����ʼ���Ʒ�걨\n", up->name);
		printf("1.����������Ա\n");
		printf("2.ɾ��������Ա\n");
		printf("3.������Ϣ��ѯ\n");
		printf("0.�˳���¼\n");
		printf("��ѡ���ܣ�");
		scanf("%d", &i);
		getchar();
		switch (i)
		{
		case 1:
			u_head = sign_up(1, u_head);
			break;
		case 2:
			u_head = del_user(1, u_head);
			break;
		case 3:
			up = user_set_up(up, u_head);
			if (up == NULL)
				return;
			break;
		case 0:
			return;
		default:
			printf("�������������ѡ��");
			break;
		}
	}
}