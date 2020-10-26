#define maxsize 100
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

// 菜品 结构体
struct kinds{
	int id;			
	char name[20];  
	float price;	
	float num;      
};

// 仓库 结构体
struct store{
    // food 里面数据的条数
	int len;
	struct kinds food[maxsize]; 
};

// 字符串判断
int stringEqual(char* s1, char* s2);

// 初始化数据
void initDataBase();

// 属性菜单头
void itemProperty();

// 指令
void commander(store *p);

// 应用初始胡
void initApp();

// 有垃圾 报错
void commandError(store *p);

// 垃圾信息收集
static char rubbish[256] = "\0";

// 判断一个字符串是否全为空格
int isAllSpace(char s[]);

// 错误和帮助提示
void helpNotion();
void helper();

// 用户
typedef struct User {
    int id;
    char name[20];
    char password[20];
};

// int loginStatus = 0;

void
log(char* message, int n) {
    printf("%s %d", message, n);
}

void
dudu() {
	printf(">>> ");
}

// split 函数 切割字符串
char**
split(const char *source, char flag) {
    char **pt;
    int j, n = 0;
    int count = 1;
    int len = strlen(source);
    char tmp[len + 1];
    tmp[0] = '\0';
    for(int i = 0; i < len; i++) {
        if (source[i] == flag && source[i + 1] == '\0') {
            continue;
        } else if (source[i] == flag && source[i+1] != flag) {
            count++;
        }
    }
    pt = (char**)malloc((count + 1) * sizeof(char*));
    count = 0;
    for (int i = 0; i < len; ++i) {
        if (i == len - 1 && source[i] != flag) {
            tmp[n++] = source[i];
            tmp[n] = '\0';  
            j = strlen(tmp) + 1;
            pt[count] = (char*)malloc(j * sizeof(char));
            strcpy(pt[count++], tmp);    
        } else if(source[i] == flag) {
            j = strlen(tmp);
            if (j != 0) {
                tmp[n] = '\0';  
                pt[count] = (char*)malloc((j + 1) * sizeof(char));
                strcpy(pt[count++], tmp);
                n = 0;
                tmp[0] = '\0';
            }
        } else {
            tmp[n++] = source[i];
        }
    }
    pt[count] = NULL;
    return pt;
}

// 新增用户，返回结构体
User
newUser(int id, char* name, char* pwd) {
    User u;
    u.id = id;
    strcpy(u.name, name);
    strcpy(u.password, pwd);
    // log("id", u.id);
    // log(u.name, -1);
    // log(u.password, -1);
    return u;
}

// 获取结构体保存到 数据库
void
saveUser(int id, char* name, char* pwd) {
    FILE *file;
    User u = newUser(id, name, pwd);
    char* adddress = "UserData.txt";
    file = fopen(adddress, "a");
    if((file = fopen(adddress, "a")) == NULL) {
        printf("File open failed!\n");
        exit(0);
    }
    fprintf(file, "%d %s %s \n", u.id, u.name, u.password);
    if(fclose(file)) {
        printf("File close failed!\n");
        exit(0);
    }
}

// 用户登录匹配 name 和 password 
int
find(char* name, char* pwd) {
    int flag = 0;
    static char month[][256] = {};
    char buf[256];  
    FILE *file;      
    int len;             
    char* adddress = "UserData.txt";
    file = fopen(adddress, "r");
    if(file == NULL) {
        perror("Fail to read");
        exit(1);
    }
    int i = 0;
    while(fgets(buf, 1000, file) != NULL) {
        len = strlen(buf);
        // 去掉换行符
        buf[len - 1] = '\0';  
        // printf("%s", buf);
        strcpy(month[i], buf);
        i++;
    }
    // printf("%s", month[0]);
    // return *month;
    char **p1;
    for(int j = 0; j < i; j++) {
        p1 = split(month[j], ' ');
        // printf("p1 %s", p1[0]);
        if(strcmp(p1[2], pwd) == 0 && strcmp(p1[1], name) == 0) {
            flag = 1;
        }
    }
    return flag;
}

// 用户登录函数
void
login() {
    printf("Login begin\n");
    char name[20]; 
    char pwd[20];
    printf("UserName: ");
    scanf("%s", &name);
	gets(rubbish);
	if(!stringEqual("\0", rubbish)) {
		strcpy(rubbish, "\0");
		printf("UserName Syntax Wrong\n");
		initApp();
		return;
	} 
    printf("Password: ");
    scanf("%s", &pwd);
	gets(rubbish);
	if(!stringEqual("\0", rubbish)) {
		strcpy(rubbish, "\0");
		printf("Password Syntax Wrong\n");
		initApp();
		return;
	} 
    int check = find(name, pwd);
    if(check) {
        printf("Login Success\n");
		initDataBase();
    } else {
        printf("Login Fail\n");
		initApp();
    }
}

// 获取最大的用户 id，用于用户 id 自增使用
int
getCurId() {
    static char month[][256] = {};
    char buf[256];  
    FILE *file;      
    int len;             
    char* adddress = "UserData.txt";
    file = fopen(adddress, "r");
    if(file == NULL) {
        perror("Fail to read");
        exit(1);
    }
    int i = 0;
    while(fgets(buf, 1000, file) != NULL) {
        len = strlen(buf);
        // 去掉换行符
        buf[len - 1] = '\0';  
        // printf("%s", buf);
        strcpy(month[i], buf);
        i++;
    }
    // printf("%s", month[0]);
    // return *month;
    char **p1;
    for(int j = 0; j < i; j++) {
        p1 = split(month[j], ' ');
    }
    int a = 0;
    sscanf(p1[0], "%d", &a);
    return a;
}

// 用户注册函数
void
regist() {
    printf("Begin Regist\n");
    int maxId = getCurId();
    int curId = maxId + 1;
    char name[20]; 
    char pwd[20];
    printf("UserName: ");
	scanf("%s", &name);
	gets(rubbish);
	if(!stringEqual("\0", rubbish)) {
		strcpy(rubbish, "\0");
		printf("UserName Syntax Wrong\n");
		initApp();
		return;
	} 
    printf("password: ");
	scanf("%s", &pwd);
	gets(rubbish);
	if(!stringEqual("\0", rubbish)) {
		strcpy(rubbish, "\0");
		printf("Password Syntax Wrong\n");
		initApp();
		return;
	} 
    saveUser(curId, name, pwd);
	printf("Regist Success\n");
	initApp();
}

// 2020/09/24

// 新增数据库功能
// 使用的是 txt 作为我们的数据库，后面可以使用 dat 文件作为数据库
// 原理都一样，最重要的是要先掏出来

// 判断字符串是否相等
int
stringEqual(char* s1, char* s2) {
    return strcmp(s1, s2) == 0;
}

// 将 数据库 置空
void
emptyData() {
	FILE *f;
	f = fopen("ItemData.txt", "w");
	fclose(f);
}

// 获取 菜品 的 最大 id，便于自增
// 这个是通过获取 数据库 文件来获取的 id
// 因为现在已经实现了 把数据库里的数据 传入了结构体中
// 所以我们可以直接获取 结构体里面的 len
int
getCurItemId() {
    static char month[][256] = {};
    char buf[256];  
    FILE *file;      
    int len;             
    char* adddress = "ItemData.txt";
    file = fopen(adddress, "r");
    if(file == NULL) {
        perror("Fail to read");
        exit(1);
    }
    int i = 0;
    while(fgets(buf, 1000, file) != NULL) {
        len = strlen(buf);
        buf[len - 1] = '\0';  
        strcpy(month[i], buf);
        i++;
    }
    char **p1;
    for(int j = 0; j < i; j++) {
        p1 = split(month[j], ' ');
    }
    int a = 0;
    sscanf(p1[0], "%d", &a);
    return a;
}

// 获取数据库中有多少条数据（从数据库载入到结构体时，并不知道有多少数据，无法判断循环次数）
int 
GetDataLine(){
    FILE *fd;
    int count = 0;
    if (fd = fopen("ItemData.txt", "r")) {
        while (!feof(fd)) {
            if ('\n' == fgetc(fd)) {
                count ++;
            }
        }
    }
    // printf("count: %d\n", count);
    if(fd) {
        fclose(fd);
    }
    return count;
}

// 初始化操作
void 
initialize(store *p){
	p->len = 0;
}	

// 向 数据库 中存入 一条数据
void
saveFood(int id, char* item, float price, float num) {
	FILE *file;
    char* adddress = "ItemData.txt";
    file = fopen(adddress, "a");
    if((file = fopen(adddress, "a")) == NULL) {
        printf("File open failed!\n");
        exit(0);
    }
    fprintf(file, "%d %s %f %f \n", id, item, price, num);
    if(fclose(file)) {
        printf("File close failed!\n");
        exit(0);
    }
} 

// 将 结构体 所有数据 存入数据库中
void
saveFoods(store *p) {
	// 因为要重写数据库，所以这里我们先置空数据库
	emptyData();
	// 开始保存数据
	// printf("enter saveFood\n");
    int len = p->len;
    char* itemName;
	// printf("len %d\n", len);
    for(int i = 0; i < len; i++) {
		if(p->food[i].id != -1) {
			// printf("p->food[i].id %d\n", p->food[i].id);
			FILE *file;
			char* adddress = "ItemData.txt";
			file = fopen(adddress, "a");
			if((file = fopen(adddress, "a")) == NULL) {
				printf("File open failed!\n");
				exit(0);
			}
			fprintf(file, "%d %s %f %f \n", p->food[i].id, p->food[i].name, p->food[i].price, p->food[i].num);
			if(fclose(file)) {
				printf("File close failed!\n");
				exit(0);
			}
		}
    }
	printf("save success\n");
}

// 读取 数据库 中的数据存入 结构体 中
void
loadToStore(store *p) {
	static char month[][256] = {};
    char buf[256];  
    FILE *file;      
    int len;             
    char* adddress = "ItemData.txt";
    file = fopen(adddress, "r");
    if(file == NULL) {
        perror("Fail to read");
        exit(1);
    }
    int i = 0;
    while(fgets(buf, 1000, file) != NULL) {
        len = strlen(buf);
        buf[len - 1] = '\0';  
        strcpy(month[i], buf);
        i++;
    }

    char **p1;
	for(int j = 0; j < GetDataLine(); j++) {
		p1 = split(month[j], ' ');
        
        int intId = 0;
        float price = 0;
		float left = 0;
        sscanf(p1[0],"%d",&intId); 
		sscanf(p1[2],"%f",&price); 
		sscanf(p1[3],"%f",&left); 	

        p->food[j].id = intId;
		strcpy(p->food[j].name, p1[1]);
        p->food[j].price = price;
        p->food[j].num = left;
		// printf("food id:%d\n", p->food[j].id);
		// printf("food price:%f\n", p->food[j].price);
		// printf("food name:%s\n", p->food[j].name);
		// printf("food left:%f\n", p->food[j].num);
        p->len += 1;
	}
}

// 用户输入数据，存入数据库
// n 为输入的数据个数
// 修改 写入结构体，最后在保存
void 
input(store *p, int n) {
    int i, j;
	// printf("len %d\n", p->len);
	int len = p->len;
    if(p->len >= maxsize - n) {
    	printf("database is empty\n");
    	//password();
		commander(p);
	} else {
		// dudu();
		// printf("getCurItemId %d\n", getCurItemId());
    	for(i = len; i < len + n; i++) {
			int j = i - len + 1;
			p->food[i].id = getCurItemId() + j;
			// printf("i: %d\n", i);
			printf("Item Name: ");
			scanf("%s", &p->food[i].name);
			printf("Item Price: ");
			scanf("%f", &p->food[i].price);
			printf("Item Left: ");
			scanf("%f", &p->food[i].num);
			p->len += 1;
			printf("-------------------\n");
    	}
    	//password();
		saveFoods(p);
		commander(p);
	}
    
}		

// 标题头
void
itemProperty() {
	printf("\n");
    printf("               Id");
	printf("               Item");
	printf("            Price");
	printf("              Left\n");
}

// 读取结构体数据，并显示出来
void
loadData(store *p) {
	// printf("load data\n");
    int len = p->len;
    // printf("len %d\n", len);
    char itemName[20] = {};
    itemProperty();
    // printf("id %d\n", p->food[0].id);
    // printf("id 3: %d\n", p->food[3].id);
    for(int i = 0; i < len; i++) {
		int* itemId = &p->food[i].id;
		float* itemPrice = &p->food[i].price;
		float* itemLeft = &p->food[i].num;
		strcpy(itemName, p->food[i].name);
		// printf("id %d\n", *itemId);
		// printf("itemName %s\n", itemName);
		// printf("itemPrice %f\n", *itemPrice);
		// printf("itemLeft %f\n", *itemLeft);
		printf("%17d %18s %16.2f %17.2f\n", *itemId, itemName, *itemPrice, *itemLeft);
    }
	printf("\nLoad data\n");
	commander(p);
}
 
// 读取数据库数据，输出 txt 文件
// 读取结构体里的数据，生成 txt 文件
void 
print(store *p, char* fileName) {
	// printf("enter print\n"); 
    int len = p->len;
    if(len <= 0) {
    	printf("Command Wrong\n"); 
    	//password();
	} else {
        FILE* data;
        data = fopen(fileName, "w");
        fclose(data);
        data = fopen(fileName, "a");
        if((data = fopen(fileName, "a")) == NULL) {
            printf("File open failed!\n");
            exit(0);
        }
        fprintf(data, "%17s %21s %23s %21s\n", "id", "item", "weight", "left");
        char itemName[20] = {};
        for(int i = 0; i < len; i++) {
            int* itemId = &p->food[i].id;
            float* itemPrice = &p->food[i].price;
            float* itemLeft = &p->food[i].num;
            strcpy(itemName, p->food[i].name);  	
            fprintf(data, "%17d %21s %15f %18f\n",*itemId, itemName, *itemPrice, *itemLeft);
        }
        if(fclose(data)) {
            printf("File close failed!\n");
            exit(0);
        }
	} 
}	

// 文档说明：
/*  1，int stringEqual(char* s1, char* s2) 判断字符串是否相等，简化代码
    2，void itemProperty() 输出行属性栏，函数复用
    3，void loadToStore(store *p) 将 数据库 中的数据载入 结构体中
    4，void saveFoods(store *p) 将 结构体 的数据 重写到 数据库中
    5，void getCurItemId() 获取当前最大 id
    6，void input(store *p, int n) 用户输入数据到结构体中，然后载入到数据库中
        n 是输入数据的个数
    7，void print(store *p, char* fileName) 用户打印数据库里的数据 
        fileName 是打印出数据的文件名 例如：data.txt
    8，void loadData(store *p) 读取数据库里的数据，然后在屏幕上显示出来
*/
// -----------------------------------------------------
// 以下的操作还需要修改
/*  1，操作完 结构体 里的数据之后，需要载入数据库
        利用 savaFoods(p)，将 结构体 里的数据载入 数据库

    2，行属性栏 把
        printf("               编号");
        printf("               名称");
        printf("       价格(rmb/kg)");
        printf("         数量(kg)\n");
        替换为 itemProperty();
        我将 这四条语句包成了 一个函数

    3，删除时 不涉及 id 的修改，id 不应该变

    4，代码规范问题，多规范一下代码，参看上方
*/ 

// 整理仓库将重名的货物排到一起
void  
clear(struct store *p) {
    int len = p->len;
    char itemName1[20] = {};
    char itmeName2[20] = {};
	for(int i = 0; i < len - 1; i++) {
		for(int j = i + 1; j < len; j++) {
            strcpy(itemName1, p->food[i].name);
            strcpy(itmeName2,  p->food[j].name);
			if(stringEqual(itemName1, itmeName2)){
                float* itemNum1 = &p->food[i].num;
                float* itemNum2 = &p->food[j].num;
				*itemNum1 += *itemNum2;
				for(int k = j; k < len - 1; k++){
					p->food[k] = p->food[k + 1];
					p->food[k].id--;
				} 
				p->len--;
			}
		}
	}
}

// 输出编号等于 ide 的数据项 
void 
printId(store *p, int ide) {
	if(p->len <= 0 || ide < 0 || ide >= p->len) {
		printf("Command Wrong\n");
		//password();
	} else {
		if(ide >= p->len) {
			printf("Not Found Data\n");
			commander(p);
			//password();
		} else {
			itemProperty();
    		printf("%17d %18s %16.2f %17.2f\n", p->food[ide].id, p->food[ide].name, p->food[ide].price, p->food[ide].num);
    		//password();
			commander(p);			
		}
	}
}	

// 输出名字等于 names 的数据项
void 
printName(store *p, char names[]) {
	int n = 0;
	if(p->len <= 0) {
		printf("Command Wrong\n");
		commander(p);			
		//password();
	} else {
		for(int i = 0; i < p->len; i++) {
			if(stringEqual(p->food[i].name, names)){
				n++;
				if(n == 1) {
					itemProperty();
				}	
    			printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
    			//password();
				commander(p);			
			}
		}
		printf("\nFind %d Valid Piece Of Data\n", n);
		commander(p);			
		//password();
	} 
}	

// v 的三种情况小于-1，等于 0，大于 1.输出价格 v prices 的数据项 
void 
printPrice(store *p, float prices, int v) {
	int n = 0;
	if(p->len<=0){
		printf("Command Wrong\n"); 
		//password();
		commander(p);
	} else {
		if(v == -1){
			for(int i = 0; i < p->len; i++) {
				if(p->food[i].price < prices) {
					n++;
					if(n == 1) {
						itemProperty();
					}
    				printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
				}
			}
			printf("\nFind %d Valid Piece Of Data\n", n);
			n = 0;
			//password();
			commander(p);
		}
		if(v == 0) {
			for(int i = 0; i < p->len; i++) {
				if(p->food[i].price == prices){
					n++; 
					if(n == 1) {
						itemProperty();
					}
    				printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
				}
			}
			printf("\nFind %d Valid Piece Of Data\n", n);
			n = 0;
			//password();
			commander(p);
		}
		if(v == 1){
			for(int i = 0; i < p->len; i++){
				if(p->food[i].price > prices){
					n++;
					if(n==1) {
						itemProperty();
					}
    				printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
				}
			}
			printf("\nFind %d Valid Piece Of Data\n", n);
			n = 0;
			//password();
			commander(p);
		}
		
	}
}	

// v的三种情况小于 -1，等于 0，大于 1，输出数量 v nums 的数据项 
void 
printNum(store *p, float nums, int v) {
	int n = 0;
	if(p->len <= 0){
		printf("Command Wrong\n"); 
		commander(p);
		//password();
	} else {
		if(v == -1) {
			for(int i = 0; i < p->len; i++) {
				if(p->food[i].num < nums) {
					n++;
					if(n==1) {
						itemProperty();
					}
    				printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
				}
			}
			printf("\nFind %d Valid Piece Of Data\n", n);
			n = 0;
			commander(p);
			//password();
		}
		if(v == 0){
			for(int i = 0; i < p->len; i++) {
				if(p->food[i].num == nums) {
					n++; 
					if(n == 1) {
						itemProperty();
					}
    				printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
				}
			}
			printf("\nFind %d Valid Piece Of Data\n", n);
			n = 0;
			commander(p);
			//password();
		}
		if(v == 1) {
			for(int i = 0;i < p->len; i++) {
				if(p->food[i].num > nums) {
					n++;
					if(n == 1) {
						itemProperty();
					}
    				printf("%17d %18s %16.2f %17.2f\n",p->food[i].id,p->food[i].name,p->food[i].price,p->food[i].num);
				}
			}
			printf("\nFind %d Valid Piece Of Data\n", n);
			n = 0;
			commander(p);
			//password();
		}	
	}
}	

// v为 -1，1 分别是修改编号为 ide 的价格和库存量
void 
changeId(store *p, int ide, float xx, int v) {
	// printf("change id %d", ide);
	// printf("float %f", xx);
	// printf("float v %d", v);
	// printf("len %d", p->len);
	if(p->len <= 0 || ide > getCurItemId() || ide < 0) {
		printf("Command Wrong\n");
		// password();
		commander(p);
	} else {
		if(v==-1) {
			for(int i = 0; i < p->len; i++) {
				if(ide == p->food[i].id) {
					p->food[i].price = xx;
					printf("Change Id Success\n");
				}
			}
			//password();
			commander(p);
		} else if(v == 1) {
			for(int i = 0; i < p->len; i++) {
				if(ide == p->food[i].id) {
					p->food[i].num = xx;
					printf("Change Id Success\n");
				}
			}
			//password();
			commander(p);
		} else {
			printf("Change Id Wrong\n");
			//password();
			commander(p);
		}
	}
	saveFoods(p);
}		

// v为 -1，1 分别是修改名字为 names[] 的价格和库存量
void 
changeName(store *p, char names[], float xx, int v) {
	int n = 0;
	if(p->len <= 0) {
		printf("Command Wrong\n");
		//password();
		commander(p);
	} else {
		for(int i = 0; i < p->len; i++) {
			if(stringEqual(p->food[i].name, names)) {
				if(v == -1) {
					n++;
					p->food[i].price = xx;
				} else if(v == 1) {
					n++;
					p->food[i].num = xx;
				}
			}
		}
		if(n > 0) {
			printf("Change Name Success\n");
			//password();
			commander(p);
		} else {
			printf("Change Name Wrong\n");
			//password();
			commander(p);
		}	
	}
	saveFoods(p);
}		

// 删除编号为 id 的货物数据
void 
deleteId(store *p, int ide) {
	int flag = 0;
	if(p->len <= 0 || ide < 0 || ide > getCurItemId()){
		printf("Command Wrong\n");
		commander(p);
	} else{
		for(int i = 0; i < p->len; i++) {
			// printf("p->food.id %d\n", p->food[i].id);
			if(p->food[i].id == ide) {
				// printf("find id\n");
				for(int j = 0; j < p->len; j++) {
					if(p->food[j].id == ide) {
						for(int i = j; i < p->len - 1; i++) {
							p->food[i] = p->food[i + 1];
						}
						flag++;
						break;
					}
				}
				
			}
		}
		if(flag > 0) {
			printf("Delete Id Success\n");
			p->len--;
			saveFoods(p);
			commander(p);
		} else {
			printf("Delete Id Wrong\n");
			commander(p);
		}
	}
} 		

// 删除名字为 name[] 的货物
void 
deleteName(store *p, char names[]) {
    int len = p->len;
	int n = 0;
	int flag = -2;
	if(len <= 0) {
		printf("Command Wrong\n");
		commander(p);
	} else {
		for(int i = 0; i < p->len; i++) {
			if(stringEqual(p->food[i].name, names)) {
				n++;
				for(int j = i; j < p->len - 1; j++) {
					p->food[j] = p->food[j + 1];
				}
				p->len--;
			}
		}
		if(n > 0) {
			printf("Delete Name Success\n");
			commander(p);
			saveFoods(p);
		} else {
			printf("Delete Name Wrong\n");
			commander(p);
		}
	}
}			

// 初始化数据库
void
initDataBase() {
	struct store ffood;
    initialize(&ffood);
	loadToStore(&ffood);
	commander(&ffood);
}

int
isAllSpace(char s[]) {
	int flag = 1;
	for(int i = 0; i < strlen(s); i++){
		if(s[i]!=' ') {
			flag = 0;
		}
	}
	return flag;
}


void
initApp() {
	dudu();
	char c[20] = {};
	scanf("%s", &c);
	strlwr(c);
	gets(rubbish);
	if(stringEqual("login", c)) {
		// puts(rubbish);
		// printf("login was compared\n");
		if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
			login();
		} else {
			// printf("Command Wrong\n");
			helpNotion();
			initApp();
		}
	} else if(stringEqual("regist", c)) {
		if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
			regist();
		} else {
			// printf("Command Wrong\n");
			helpNotion();
			initApp();
		}
	} else if(stringEqual("close", c)) {
		if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
			printf("App Is Closed\n");
		} else {
			// printf("Command Wrong\n");
			helpNotion();
			initApp();
		}
	} else if(stringEqual("help", c)) {
		if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
			helper();
			initApp();
		} else {
			helpNotion();
			initApp();
		}
	} else {
		// printf("Command Wrong\n");
		helpNotion();	
		initApp();
	}
}

int
findObj(char* obj) {
    int condition1 = stringEqual(obj, "left");
    int condition2 = stringEqual(obj, "price");
    // printf("findObj condition %d\n", condition1 || condition2);
    return condition1 || condition2;
}

int
findOp(char* op) {
    int condition1 = stringEqual(op, ">");
    int condition2 = stringEqual(op, "<");
    int condition3 = stringEqual(op, "=");
    int condition = condition1 || condition2 || condition3;
    // printf("findOp condition %d\n", condition);
    return condition;
}

int
getOp(char* op) {
    int cond1 = stringEqual(op, ">");
    int cond2 = stringEqual(op, "<");
    int cond3 = stringEqual(op, "=");
    if(cond1) {
        return 1;
    } 
    if(cond2) {
        return -1;
    } 
    if(cond3) {
        return 0;
    } 
}

int
getObj(char* obj) {
    int cond1 = stringEqual(obj, "left");
    int cond2 = stringEqual(obj, "price");
    if(cond1) {
        return 1;
    } 
    if(cond2) {
        return -1;
    } 
}

void
commandError(store* p) {
	strcpy(rubbish, "\0");
	printf("Command Wrong\n");
	commander(p);
}

void
helper() {
	printf("You can input \"login\" to enter the system.\n\n");
	printf("If you don't have any account, you can input \"login\" to regist.\n\n");
	printf("If you want to get data, you can input: \n");
	printf("\"get all\" to get all data or get with condition \"get [Property] [Operator] [Number]\", like \"get price > 10\".\n\n");
	printf("If you want to del data, you can input: \n");
	printf("\"del id [Number]\" or \"del name [String]\" to del data.\n\n");
	printf("If you want to create data, you can input: \n");
	printf("\"new [Number]\" the number is how much data you want to put in.\n\n");
	printf("If you want to change data, you can input: \n");
	printf("\"change [Property] id [IdNumber] [Number]\" or \"change [Property] name [NameString] [Number]\", like \"change price id 4 17.5\".\n");
}

void
helpNotion() {
	printf("Command Wrong\n");
	printf("You can input \"help\" to get some useful information\n");
}

void
commander(store *p) {
    /* 
        command: 
            inputData
            print
            get
            change
            del
    */ 
    int intNum;
    float fNum1;
    int idNum1;
    char command[20] = {};
    char nameS1[20] = {};
    char obj[20] = {};
    char property[20] = {};
    char operater[20] = {};
	dudu();
    scanf("%s", &command);
	strlwr(command);
    if(stringEqual(command, "new")) {
        // scanf("%s", &obj);
        // printf("inputData");
        scanf("%d", &intNum);
		gets(rubbish);
		if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
			input(p, intNum);
		} else {
			commandError(p);
		}
    } else if(stringEqual(command, "get")) {
        // printf("get");
        // get price > **
        // get all
        scanf("%s", &obj);
		strlwr(obj);
        if(stringEqual(obj, "all")) {
			gets(rubbish);
			// puts(rubbish);
			if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
				loadData(p);
			} else {
				commandError(p);
			}
        } else {
			if(findObj(obj)) {
				scanf("%s", &operater);
				if(findOp(operater)) {
					scanf("%f", &fNum1);
					gets(rubbish);
					if(stringEqual(obj, "price")) {
						if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
							printPrice(p, fNum1, getOp(operater));
						} else {
							commandError(p);
						}
					} else {
						if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
							printNum(p, fNum1, getOp(operater));
						} else {
							commandError(p);
						}
					}
					// printf("%s %s %s %f", command, obj, operater, fNum1);
				} else {
					printf("Not Found Operator\n");
					gets(rubbish);
					commander(p);
				}
			} else {
				printf("Not Found Obj\n");
				gets(rubbish);
				commander(p);
			}
		}
    } else if(stringEqual(command, "change")) {
        // printf("change");
        // change price id ** **
        scanf("%s", &obj);
		strlwr(obj);
        if(findObj(obj)) {
            scanf("%s", &property);
			strlwr(property);
            if(stringEqual(property, "id")) {
                scanf("%d", &idNum1);
                scanf("%f", &fNum1);
				gets(rubbish);
				if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
					changeId(p, idNum1, fNum1, getObj(obj));
				} else {
					commandError(p);
				}
                // printf("%s %s %s %d %f", command, obj, property, idNum1, fNum1);
            } else if(stringEqual(property, "name")) {
                scanf("%s", &nameS1);
                scanf("%f", &fNum1);
				gets(rubbish);
				if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
					changeName(p, nameS1, fNum1, getObj(obj));					
				} else {
					commandError(p);
				}
                // printf("%s %s %s %s %f", command, obj, property, nameS1, fNum1);
            } else {
                printf("Not Found Property\n");
				gets(rubbish);
				commander(p);
            }
        } else {
            printf("Not Found Obj\n");
			gets(rubbish);
			commander(p);
        }
        // printf("%s %s\n", command, obj);
    } else if(stringEqual(command, "del")) {
        // del id **
        // printf("del");
        scanf("%s", &property);
		strlwr(property);
         if(stringEqual(property, "id")) {
                scanf("%d", &idNum1);
				gets(rubbish);
				if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
					deleteId(p, idNum1);
				} else {
					commandError(p);
				}
                // printf("%s %s %d", command, property, idNum1);
            } else if(stringEqual(property, "name")) {
                scanf("%s", &nameS1);
				strlwr(nameS1);
				gets(rubbish);
				if(stringEqual("\0", rubbish) || isAllSpace(rubbish)) {
					deleteName(p, nameS1);
				} else {
					commandError(p);
				}
                // printf("%s %s %s", command, property, nameS1);
            } else {
                printf("Not Found Property\n");
				gets(rubbish);
				commander(p);
            }
    } else if(stringEqual(command, "close") || isAllSpace(rubbish)) {
        printf("App Is Closed\n");
		gets(rubbish);
    } else if(stringEqual(command, "logout") || isAllSpace(rubbish)) {
		printf("User Logged Out\n");
		gets(rubbish);
		initApp();
	} else if(stringEqual(command, "help") || isAllSpace(rubbish)) {
		helpNotion();
		gets(rubbish);
		initApp();
	} else {
        helpNotion();
		gets(rubbish);
		commander(p);
    }
}

int 
main() {
	// char x[20];
	// gets(x);
	// int y = isAllSpace(x);
	// printf("y=%d", y);
	initApp();
	// initDataBase();
	// GetDataLine();
    // struct store ffood;
    // initialize(&ffood);
	// // saveFood(0, "vegetable1", 1.5, 3.4);
	// loadToStore(&ffood);
	// // print(&ffood, "dududu.txt");
	// // input(&ffood, 1);
    // loadData(&ffood);
    return 0;
}

// 说明文档
/*	
	用户函数模块：
		1，login()
			登录函数
			登录成功后 开始 初始化结构体，载入数据
		2，regist()
			注册函数
	仓库函数模块：
		1，int stringEqual(char* s1, char* s2) 
			判断字符串是否相等，简化代码
		2，void itemProperty() 
			输出行属性栏，函数复用
		3，void loadToStore(store *p) 
			将 数据库 中的数据载入 结构体中
		4，void saveFoods(store *p) 
			将 结构体 的数据 重写到 数据库中
		5，void getCurItemId() 
			获取当前最大 id
		6，void GetDataLine()
			获取数据库的数据个数，载入数据的时候使用
		7，void input(store *p, int n) 
			用户输入数据到结构体中，然后载入到数据库中
			n 是输入数据的个数
		8，void print(store *p, char* fileName) 
			用户打印所有数据库里的数据 
			fileName 是打印出数据的文件名 例如：data.txt
		9，void loadData(store *p) 
			读取数据库里的数据，然后在屏幕上显示出来
		10，void printId(store *p, int ide) 
			将 相应 id 的数据显示出来（未作输出文件功能）
		11，void printName(store *p, char names[])
			将 相应 name 的数据显示出来（未作输出文件功能）
		12，void printPrice(store *p, float prices, int v)
			将 相应 price 的数据显示出来（未作输出文件功能）
			v 的三种情况小于-1，等于 0，大于 1，输出价格 v prices 的数据项 
		13，void printNum(store *p, float nums, int v)
			将 相应 num 的数据显示出来（未作输出文件功能）
			v的三种情况小于 -1，等于 0，大于 1，输出数量 v nums 的数据项 
		14，void changeId(store *p, int ide, float xx, int v)
			改变相应 id 的数据
			v 为 -1，1 分别是修改编号为 ide 的价格和库存量
		15，void changeName(store *p, char names[], float xx, int v)
			改变相应 name 的数据
			v 为 -1，1 分别是修改名称为 name 的价格和库存量
		16，void deleteId(store *p, int ide)
			删除 相应 ide 的数据
		17，void deleteName(store *p, char names[])
			删除 相应 name 的数据
*/
