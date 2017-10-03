#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <algorithm>
std::map<std::string, std::string> eval(std::vector<std::string> vec, std::map<std::string, std::string> locals);
std::string exp_eval(std::string expression, std::map<std::string, std::string> locals);
bool use_unsafe=false;
std::map<std::string, std::string> variables;
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
eval(s, std::map<std::string, std::string>{});
}
void lang_func_use(std::string& funcname, std::string& pre_func_args, std::map<std::string, std::string>locals)
{
std::vector<std::string> func_args;
std::map<std::string, std::string> pre_locals;
std::stringstream f(pre_func_args);
int i=0;
while (getline(f,pre_func_args, ','))
{
pre_locals[function_args[funcname][i]]=exp_eval(pre_func_args, locals);
i++;
}
//std::cout << funcname;
//for(int i=0;i<functions[funcname].size();i++){std::cout << functions[funcname][i];}
//for(int i=0;i<function_args[funcname].size();i++){std::cout << function_args[funcname][i] << " = " << pre_locals[function_args[funcname][i]];}

eval(functions[funcname], pre_locals);
}
std::string exp_eval(std::string expression, std::map<std::string, std::string> locals)
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
tokens[x]["string"].assign(locals[varname]);
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
tokens[x]["string"].assign(variables[varname]);
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
std::map<std::string, std::string> eval(std::vector<std::string> vec, std::map<std::string, std::string> locals)
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
	lang_print(exp_eval(match[1], locals));
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
	std::string pre_func_args;
	std::stringstream f(match[2]);
	while (getline(f,pre_func_args, ','))
	{
	if(pre_func_args[0]!='$'){std::cout << "Error: Arguments must be local to scope\n"; if(use_unsafe){std::cout << "_use_unsafe: overiding...\n";function_args[funcname].push_back('$'+pre_func_args);}else{exit(0);}}
	function_args[funcname].push_back(pre_func_args);
	}
	functions[funcname]=f_vec;
//	for (int z=0; z<functions["wordt"].size();z++){std::cout << f_vec[z];}
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
	is_true=exp_eval((std::string)match[1],locals)==exp_eval((std::string)match[2],locals);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)!=(.*)")))
	{
	is_true=exp_eval((std::string)match[1],locals)!=exp_eval((std::string)match[2],locals);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)>(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals))>std::stoi(exp_eval((std::string)match[2],locals));	
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)<(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals))<std::stoi(exp_eval((std::string)match[2],locals));
	}
	else{is_true=std::stoi(exp_eval(expr, locals));}
	if (is_true){locals=eval(f_vec, locals);}
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
	is_true=exp_eval((std::string)match[1],locals)==exp_eval((std::string)match[2],locals);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)!=(.*)")))
	{
	is_true=exp_eval((std::string)match[1],locals)!=exp_eval((std::string)match[2],locals);
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)>(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals))>std::stoi(exp_eval((std::string)match[2],locals));	
	}
	else if(std::regex_search(expr, match, std::regex ("(.*)<(.*)")))
	{
	is_true=std::stoi(exp_eval((std::string)match[1],locals))<std::stoi(exp_eval((std::string)match[2],locals));
	}
	else{is_true=std::stoi(exp_eval(expr, locals));}
	if (is_true){locals=eval(f_vec, locals);}
	else{break;}
	}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*(.*)=(.*)")))
	{
	if(((std::string)match[1])[0]=='$'){locals[match[1]]=exp_eval(match[2], locals);}
	else{if(((std::string)match[1])[0]>=48 && ((std::string)match[1])[0]<=57){std::cout<<"SyntaxError: Global variables can't start with integer\n";if(use_unsafe){std::cout << "_use_unsafe: fatal\n";}exit(0);}variables[match[1]]=exp_eval(match[2], locals);}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*break"))){return locals;}
	else if (std::regex_search(input, match, std::regex ("^\\s*strcat: (.*) (.*)")))
	{
	if(((std::string)match[1])[0]=='$'){locals[match[1]].append(match[2]);}
	else{variables[match[1]].append(match[2]);}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*(.*)\\((.*)\\)")))
	{
	lang_func_use((std::string)match[1],(std::string)match[2], locals);
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*exec:\\s*(.*)")))
	{
	if(((std::string)match[1])[0]=='$'){eval(std::vector<std::string>{(std::string)locals[match[1]]},std::map<std::string, std::string> {});}
	else{eval(std::vector<std::string>{variables[match[1]]},std::map<std::string, std::string> {});}
	}
	else if (std::regex_search(input, match, std::regex ("^_use_unsafe$"))){use_unsafe=true;}
	else if (std::regex_search(input, match, std::regex ("^_use_safe$"))){use_unsafe=false;}
	else if (std::regex_search(input, match, std::regex ("^\\s*strvcat:\\s*(.*) (.*)")))
	{
	if(variables[match[1]][0]=='$'){locals[match[1]].append(locals[match[2]]);}
	else{variables[match[1]].append(variables[match[2]]);}
	}
	else if (std::regex_search(input, match, std::regex ("^\\s*_quit")))
	{
	std::exit(0);
	}
	else {std::cout << "SyntaxError: Unreckognized keyword or function: " << input << "\n";if (use_unsafe==false){std::exit(0);}else{std::cout <<"_use_unsafe: overriding...\n";}}
	}
	return locals;
}

int main(int argc, char *argv[])
{
	if(argc<2){std::cout << "Need input or -i"; std::exit(0);}
	if(strcmp(argv[1],"-i")==0)
	{
	std::string input;
	std::vector<std::string> in_vec;
	while (1)
	{
	std::cout << "\n>>> ";
	std::getline(std::cin, input);
	in_vec.push_back(input);
	eval(in_vec, std::map<std::string, std::string>{});
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
