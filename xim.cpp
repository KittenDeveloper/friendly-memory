#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <algorithm>
#include <tuple>
//QuickDebugPrint
void p(std::string s){std::cout<<s<<'\n';}
class Xim_Var {
	public:
		Xim_Var(std::string s_in){string_val=s_in;}
		Xim_Var(){/*PASS*/}
		std::string s_val(){/*if(value_type=="ListType"){string_val.erase();for(auto &p: elements){string_val.append(&p.string_val());}}*/return string_val;}
		void set(std::string s_in){if(value_type=="ListType"){std::cout<<"TypeError: Can't set ListType to string value\n";exit(0);}else{string_val=s_in;}}
	private:
		std::string value_type;
		std::string string_val;
		int int_val;
};
class Xim_Array {
	public:
	Xim_Array(){/*PASS*/}
	std::string s_val()
	{
	string_val.erase();
	for(auto p: elements){string_val.append(p.s_val());}
	return string_val;
	}
	std::string get(int index)
	{
	if (index > elements.size()){std::cout<<"IndexError: Index out of range\n";exit(0);}
	else{return elements[index].s_val();}
	}
	void append(Xim_Var value){elements.push_back(value);}
	private:
		std::string string_val;
		std::vector<Xim_Var> elements;
};
std::pair<std::map<std::string, Xim_Var>,std::map<std::string, Xim_Array>> eval(std::vector<std::string> vec, std::map<std::string, Xim_Var> locals, std::map<std::string, Xim_Array> local_arrays);
std::string exp_eval(std::string expression, std::map<std::string, Xim_Var> locals, std::map<std::string, Xim_Array> local_arrays);
bool use_unsafe=false;
std::map<std::string, Xim_Var> variables;
std::map<std::string, Xim_Array> arrays;
std::map<std::string, std::vector<std::string>> functions;
std::map<std::string, std::vector<std::string>> function_args;
void exec_seg(std::string content)
{
std::stringstream f(content);
std::string the_line;
std::vector<std::string> s;
while (getline(f,the_line, '\n'))
{
s.push_back(the_line);
}
eval(s, std::map<std::string, Xim_Var>{}, std::map<std::string, Xim_Array> {});
}
void lang_func_use(std::string& funcname, std::string& pre_func_args, std::map<std::string, Xim_Var>locals, std::map<std::string, Xim_Array>local_arrays)
{
std::vector<std::string> func_args;
std::map<std::string, Xim_Var> pre_locals;
std::stringstream f(pre_func_args);
int i=0;
//if(pre_func_args.find(",")!=std::string::npos)
//{
while (getline(f,pre_func_args, ','))
{
pre_locals[function_args[funcname][i]].set(exp_eval(pre_func_args, locals, local_arrays));
i++;
//}
}

//std::cout << funcname;
//for(int i=0;i<functions[funcname].size();i++){std::cout << functions[funcname][i];}
//for(int i=0;i<function_args[funcname].size();i++){std::cout << function_args[funcname][i] << " = " << pre_locals[function_args[funcname][i]];}

eval(functions[funcname], pre_locals, std::map<std::string, Xim_Array>{});
}
std::string exp_eval(std::string expression, std::map<std::string, Xim_Var> locals, std::map<std::string, Xim_Array> local_arrays)
{
//if(locals.size()>0){for (const auto &p : locals) {std::cout << "locals['" << p.first << "'] = " << p.second << '\n';}}
std::vector<std::map<std::string, std::string>> tokens;
int x=0;
std::smatch match;
expression.append(".");
for (int i=0; i<expression.size(); i++)
{
std::map<std::string, std::string> current= {};
if (expression[i]=='.'||expression[i]==' '){}
else if (expression[i]>=48 && expression[i]<=57)
{
current["string"]="";
tokens.push_back(current);
while(expression[i]>=48 && expression[i]<=57)
{
tokens[x]["string"].append(std::string(1,expression[i]));
i++;
}
i--;
//std::cout << "_new int: " << tokens[x]["int"];
x++;
}
else if(expression[i]=='$')
{
if(expression[i+1]=='@'){goto arrayhandler;}
current["string"]="";
tokens.push_back(current);
std::string varname="";
while (expression[i]!='.' && expression[i]!=' '&& expression[i]!='+'&& expression[i]!='-'&& expression[i]!='*'&& expression[i]!='/')
{
varname.append(std::string(1,expression[i]));
i++;
}
i--;
//std::cout<<locals[varname];
tokens[x]["string"].assign(locals[varname].s_val());
x++;
}
else if(expression[i]=='@')
{
arrayhandler:
//std::cout<<"FoundARR\n";
//std::cout<<expression[i];
i++;
bool islocal =expression[i]=='$';
current["string"]="";
tokens.push_back(current);
std::string varname="";
int index=-1;
std::string pre_index="";
while (i>expression.size() &&expression[i]!='.' && expression[i]!=' '&& expression[i]!='+'&& expression[i]!='-'&& expression[i]!='*'&& expression[i]!='/')
{
if(expression[i]=='[')
{
i++;
while(expression[i]>=48 && expression[i]<=57 )
{
pre_index.append(1,expression[i]);
i++;
}
index=std::stoi(pre_index);
//std::cout<<index;
}
i++;
if(expression[i]==']'){i++;break;}
varname.append(std::string(1,expression[i]));
}
--i;
if(index!=-1)
{
if(islocal){tokens[x]["string"].assign(local_arrays[varname].get(index));}
else{tokens[x]["string"].assign(arrays[varname].get(index));}
}
else
{
//std::cout<<"_index: "<<index<<"\n";
std::cout<<varname;
if(islocal){tokens[x]["string"].assign(local_arrays[varname].s_val());}
else{tokens[x]["string"].assign(arrays[varname].s_val());}
}
x++;
}
else if(expression[i]=='+'){current["add"]="";tokens.push_back(current);x++;}
else if(expression[i]=='*'){current["mul"]="";tokens.push_back(current);x++;}
else if(expression[i]=='/'){current["div"]="";tokens.push_back(current);x++;}
else if(expression[i]=='-'){current["sub"]="";tokens.push_back(current);x++;}
else if (expression[i]=='"')
{
i++;
current["string"]="";
tokens.push_back(current);
while (1)
{
if (expression[i]=='\\')
{
i++;
if (expression[i]=='n'){tokens[x]["string"].append("\n");}
else if (expression[i]=='\\'){tokens[x]["string"].append("\\");}
else if (expression[i]=='"'){tokens[x]["string"].append("\"");}
i++;
continue;
}
if (expression[i]=='"'){break;}
tokens[x]["string"].append(std::string(1,expression[i]));
i++;
}
//std::cout <<tokens[x]["string"];
x++;
}
else
{
current["string"]="";
tokens.push_back(current);
std::string varname="";
while (expression[i]!='.' && expression[i]!=' '&& expression[i]!='+'&& expression[i]!='-'&& expression[i]!='*'&& expression[i]!='/')
{
varname.append(std::string(1,expression[i]));
i++;
}
i--;
tokens[x]["string"].assign(variables[varname].s_val());
x++;
}
}
std::string finalstr;
tokens.push_back(std::map<std::string, std::string> {{"NULL",""}});
for (int i=0; i<tokens.size(); i++)
{
if (tokens[i].count("string")){
if (tokens[i+1].count("add")){finalstr.append(std::to_string(std::stoi(tokens[i]["string"])+std::stoi(tokens[i+2]["string"])));tokens[i]["string"].assign(std::to_string(std::stoi(tokens[i]["string"])+std::stoi(tokens[i+2]["string"])));tokens.erase(tokens.begin()+i+1);tokens.erase(tokens.begin()+i+1);i=-1;finalstr="";}
else if (tokens[i+1].count("sub")){finalstr.append(std::to_string(std::stoi(tokens[i]["string"])-std::stoi(tokens[i+2]["string"])));tokens[i]["string"].assign(std::to_string(std::stoi(tokens[i]["string"])-std::stoi(tokens[i+2]["string"])));tokens.erase(tokens.begin()+i+1);tokens.erase(tokens.begin()+i+1);i=-1;finalstr="";}
else if (tokens[i+1].count("mul")){finalstr.append(std::to_string(std::stoi(tokens[i]["string"])*std::stoi(tokens[i+2]["string"])));tokens[i]["string"].assign(std::to_string(std::stoi(tokens[i]["string"])*std::stoi(tokens[i+2]["string"])));tokens.erase(tokens.begin()+i+1);tokens.erase(tokens.begin()+i+1);i=-1;finalstr="";}
else if (tokens[i+1].count("div")){finalstr.append(std::to_string(std::stoi(tokens[i]["string"])/std::stoi(tokens[i+2]["string"])));tokens[i]["string"].assign(std::to_string(std::stoi(tokens[i]["string"])/std::stoi(tokens[i+2]["string"])));tokens.erase(tokens.begin()+i+1);tokens.erase(tokens.begin()+i+1);i=-1;finalstr="";}
else{finalstr.append(tokens[i]["string"]);}
}
}
//std::cout << "_" <<finalstr;
return finalstr;
}
void lang_print(const std::string& thestr)
{
std::string outstr;
std::regex_replace(std::back_inserter(outstr), thestr.begin(), thestr.end(), std::regex ("\\\\n"), "\n");
std::cout << outstr;
}
#define advance i++;input=vec[i]
std::pair<std::map<std::string, Xim_Var>, std::map<std::string, Xim_Array>> eval(std::vector<std::string> vec, std::map<std::string, Xim_Var> locals, std::map<std::string, Xim_Array> local_arrays)
{
/*
	std::map<std::string, std::string> locals;
	locals.insert(pre_locals.begin(), pre_locals.end());
	if(locals.size()>0){for (const auto &p : locals) {std::cout << "locals['" << p.first << "'] = " << p.second << '\n';}}
*/
	int rx=0;
	std::string input;
	for(unsigned i=0; i<vec.size(); i++)
	{
	input=vec[i];
	std::smatch match;
	if (std::regex_search(input, match, std::regex ("^\\s*print:(.*)")))
	{
	lang_print(exp_eval(match[1], locals, local_arrays));
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*$"))){/*PASS*/}
	else if (std::regex_search(input, match, std::regex ("\\s*arraydef:\\s*(.*)\\s*")))
	{
	std::string varname=match[1];
	if(varname[0]=='$'){local_arrays[varname]=Xim_Array();}
	else{arrays[varname]=Xim_Array();}
	}
	else if (std::regex_search(input, match, std::regex ("\\s*input:\\s*(.*)")))
	{
	std::string var_in;
	std::getline(std::cin, var_in);
	if(((std::string)match[1])[0]=='$'){locals[match[1]].set(var_in);}
	else{variables[match[1]].set(var_in);}
	}
	else if (std::regex_search(input, match, std::regex ("^function (.*)\\((.*)\\)")))
	{
	std::vector<std::string> f_vec;
	std::string funcname = match[1];
	while (input.find("{")==-1)
	{
//	if (input!="" && input.find("{")==-1){std::cout << "SyntaxError: Found string before starting {\n";if(use_unsafe){std::cout<<"use_unsafe: overriding...\n";}else{exit(0);}}
	advance;
	}
	rx=0;
	while(rx!=1)
	{
	advance;
	rx-=std::count(input.begin(), input.end(), '{');
	rx+=std::count(input.begin(), input.end(), '}');
	if(rx==1){break;}
	f_vec.push_back(input);
	}
	std::string pre_func_args=match[2];
	std::stringstream f(match[2]);
	if (pre_func_args.find(',')!=std::string::npos)
	{
	while (getline(f,pre_func_args, ','))
	{
	if(pre_func_args[0]!='$'){std::cout << "Error: Arguments must be local to scope\n"; if(use_unsafe){std::cout << "_use_unsafe: overiding...\n";function_args[funcname].push_back('$'+pre_func_args);}else{exit(0);}}
	}
	}
	else
	{
//	std::cout <<pre_func_args;
	if(pre_func_args[0]!='$')
	{
	std::cout << "Error: Arguments must be local to scope\n";
	if(use_unsafe)
	{
	std::cout << "_use_unsafe: overiding...\n";
	function_args[funcname].push_back('$'+pre_func_args);
	}
	else{exit(0);}
	}
	else{function_args[funcname].push_back(pre_func_args);}
	}
	functions[funcname]=f_vec;
//	for (int z=0; z<functions["wordt"].size();z++){std::cout << f_vec[z];}
	}
	else if (std::regex_search(input, match, std::regex ("\\s*#"))){}
	else if (std::regex_search(input, match, std::regex ("\\s*import:\\s*(.*)\\s*")))
	{
	std::ifstream ifs(match[1]);
	std::string content( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
	exec_seg(content);
	}
	else if (std::regex_search(input, match, std::regex ("\\s*if\\s*\\((.*)\\)")))
	{
	std::string expr=match[1];
	std::vector<std::string> f_vec;
	while (input.find("{")==-1)
	{
	advance;
	}
	rx=0;
	while(rx!=1)
	{
	advance;
	rx-=std::count(input.begin(), input.end(), '{');
	rx+=std::count(input.begin(), input.end(), '}');
	if(rx==1){break;}
	f_vec.push_back(input);
	}
	bool is_true;
	if(std::regex_search(expr, match, std::regex ("(.*)==(.*)")))
	{
	is_true=exp_eval((std::string)match[1],locals,local_arrays)==exp_eval((std::string)match[2],locals,local_arrays);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)!=(.*)")))
	{
	is_true=exp_eval((std::string)match[1],locals,local_arrays)!=exp_eval((std::string)match[2],locals,local_arrays);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)>(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals,local_arrays))>std::stoi(exp_eval((std::string)match[2],locals,local_arrays));	
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)<(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals,local_arrays))<std::stoi(exp_eval((std::string)match[2],locals,local_arrays));
	}
	else{is_true=std::stoi(exp_eval(expr, locals, local_arrays));}
	if (is_true){std::tie(locals,local_arrays)=eval(f_vec, locals, local_arrays);}
	}
	else if (std::regex_search(input, match, std::regex ("\\s*while\\s*\\((.*)\\)")))
	{
	std::string expr=match[1];
	std::vector<std::string> f_vec;
	while (input.find("{")==-1)
	{
	advance;
	}
	rx=0;
	while(rx!=1)
	{
	advance;
	rx-=std::count(input.begin(), input.end(), '{');
	rx+=std::count(input.begin(), input.end(), '}');
	if(rx==1){break;}
	f_vec.push_back(input);
	}
	bool is_true;
	while(1)
	{
	if(std::regex_search(expr, match, std::regex ("(.*)==(.*)")))
	{
	is_true=exp_eval((std::string)match[1],locals,local_arrays)==exp_eval((std::string)match[2],locals,local_arrays);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)!=(.*)")))
	{
	is_true=exp_eval((std::string)match[1],locals,local_arrays)!=exp_eval((std::string)match[2],locals,local_arrays);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)>(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals,local_arrays))>std::stoi(exp_eval((std::string)match[2],locals,local_arrays));	
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)<(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals,local_arrays))<std::stoi(exp_eval((std::string)match[2],locals,local_arrays));
	}
	else{is_true=std::stoi(exp_eval(expr, locals,local_arrays));}
	if (is_true){std::tie(locals,local_arrays)=eval(f_vec, locals,local_arrays);}
	else{break;}
	}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*@(.*).append\\((.*)\\)")))
	{
	std::string arrayname=match[1];
	std::string val=exp_eval((std::string)match[2], locals,local_arrays);
//	std::cout <<arrayname<<":"<<val<<'\n';
	if(arrayname[0]=='$'){local_arrays[arrayname].append(Xim_Var(val));}
	else{arrays[arrayname].append(Xim_Var(val));}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*(.*)\\s*=(.*)")))
	{
	if(((std::string)match[1])[0]=='$'){locals[match[1]]=Xim_Var(exp_eval(match[2], locals, local_arrays));}
	else{if(((std::string)match[1])[0]>=48 && ((std::string)match[1])[0]<=57){std::cout<<"SyntaxError: Global variables can't start with integer\n";if(use_unsafe){std::cout << "_use_unsafe: fatal\n";}exit(0);}variables[match[1]]=Xim_Var(exp_eval(match[2], locals, local_arrays));}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*break"))){return std::make_pair(locals,local_arrays);}
	else if (std::regex_search(input, match, std::regex ("^\\s*(.*)\\((.*)\\)")))
	{
	lang_func_use((std::string)match[1],(std::string)match[2], locals, local_arrays);
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*exec:\\s*(.*)")))
	{
	if(((std::string)match[1])[0]=='$'){eval(std::vector<std::string>{(std::string)locals[match[1]].s_val()}, locals, local_arrays);}
	else{eval(std::vector<std::string>{variables[match[1]].s_val()}, locals, local_arrays);}
	}
	else if (std::regex_search(input, match, std::regex ("^_use_unsafe$"))){use_unsafe=true;}
	else if (std::regex_search(input, match, std::regex ("^_use_safe$"))){use_unsafe=false;}
	else if (std::regex_search(input, match, std::regex ("^\\s*_quit")))
	{
	std::exit(0);
	}
	else {std::cout << "SyntaxError: Unreckognized keyword or function: " << input << "\n";if (use_unsafe==false){std::exit(0);}else{std::cout <<"_use_unsafe: overriding...\n";}}
	}
	return std::make_pair(locals,local_arrays);
}

int main(int argc, char *argv[])
{
	if(argc<2){std::cout << "Need input or -i"; std::exit(0);}
	if(strcmp(argv[1],"-i")==0)
	{
	std::string input;
	std::vector<std::string> in_vec;
	std::pair<std::map<std::string, Xim_Var>, std::map<std::string, Xim_Array>> locals{};
	while (1)
	{
	std::cout << "\n>>> ";
	std::getline(std::cin, input);
	in_vec.push_back(input);
	locals=eval(in_vec, locals.first, locals.second);
	in_vec.pop_back();
	}
	}
	else
	{
	std::ifstream ifs(argv[1]);
	std::string content( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>()) );
	exec_seg(content);
/*	for (const auto &p : functions) {
	std::cout << "functions[" << p.first << "]" << '\n';
	}
*/
	}
}