#include "stdlib.h"
#include <iostream>

//下面三个文件是本段代码需要的库文件
#include "rapidxml.hpp"       
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

int CreateXml();
int ReadAndChangeXml();
int main()
{
	//测试用例
	CreateXml();
	//测试用例
	ReadAndChangeXml();
	system("pause");
	return 0;
}
//创建一个名称为config2.xml文件
int CreateXml()
{
	rapidxml::xml_document<> doc;
#if 0
	rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
#else
	rapidxml::xml_node<>* rot = doc.allocate_node(rapidxml::node_declaration);
	rot->append_attribute(doc.allocate_attribute("version","1.0"));
	rot->append_attribute(doc.allocate_attribute("encoding","utf-8"));
#endif
	doc.append_node(rot);
	rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element,"config","information");
	doc.append_node(node);
#if 0
	rapidxml::xml_node<>* color = doc.allocate_node(rapidxml::node_element,"color",NULL);
#else
	char *node_name = doc.allocate_string("color");
	rapidxml::xml_node<>* color = doc.allocate_node(rapidxml::node_element,node_name);
#endif
	node->append_node(color);
	color->append_node(doc.allocate_node(rapidxml::node_element,"red","0.1"));
	color->append_node(doc.allocate_node(rapidxml::node_element,"green","0.1"));
	color->append_node(doc.allocate_node(rapidxml::node_element,"blue","0.1"));
	color->append_node(doc.allocate_node(rapidxml::node_element,"alpha","1.0"));
	rapidxml::xml_node<>* size = doc.allocate_node(rapidxml::node_element,"size",NULL);
	size->append_node(doc.allocate_node(rapidxml::node_element,"x","640"));
	size->append_node(doc.allocate_node(rapidxml::node_element,"y","480"));
	node->append_node(size);
	rapidxml::xml_node<>* mode = doc.allocate_node(rapidxml::node_element,"mode","screen mode");
	mode->append_attribute(doc.allocate_attribute("fullscreen","false"));
	node->append_node(mode);
	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);
	std::cout<<text<<std::endl;
	std::ofstream out("config2.xml");
	out << doc;

	return 0;
}


//读取并修改config3.xml
int ReadAndChangeXml()
{
	rapidxml::file<> fdoc("config2.xml");
	std::cout<<fdoc.data()<<std::endl;
	rapidxml::xml_document<> doc;
#if 0
	doc.parse<0>(fdoc.data()); 
#else
	//doc.parse<rapidxml::parse_no_data_nodes>(fdoc.data()); // 这种解析方式才可以用value()修改节点的值
	doc.parse<rapidxml::parse_declaration_node>(fdoc.data()); 
#endif
	std::cout<<doc.name()<<std::endl;
	//! 获取根节点
	rapidxml::xml_node<>* root = doc.first_node();
	rapidxml::node_type type =  root->type();

	while(type != rapidxml::node_element)
	{  
		root = root->next_sibling();
		type = root->type();
	}

	std::cout<<root->name()<<std::endl;
	//! 获取根节点第一个节点
	rapidxml::xml_node<>* node1 = root->first_node();
	std::cout<<node1->name()<<std::endl;
	rapidxml::xml_node<>* node11 = node1->first_node();
	std::cout<<node11->name()<<std::endl;
	std::cout<<node11->value()<<std::endl;
	//! 修改之后再次保存
	rapidxml::xml_node<>* size = root->first_node("size");
	{ // modifying
		rapidxml::xml_node<>* size1 = size->first_node("x");
		size1->name("x_modifying");
		char *node_value = doc.allocate_string("9000");
		size1->value(node_value);
	}
	size->append_node(doc.allocate_node(rapidxml::node_element,"w","1"));
	size->append_node(doc.allocate_node(rapidxml::node_element,"h","1"));
	std::string text;
	rapidxml::print(std::back_inserter(text),doc,0);
	std::cout<<text<<std::endl;
	std::ofstream out("config3.xml");
	out << doc;

	return 0;
}