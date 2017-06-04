#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace boost::property_tree;
using namespace boost;

int InsertJson()
{
	std::string str = "{\"code\":0,\"images\":[{\"url\":\"fmn057/20111221/1130/head_kJoO_05d9000251de125c.jpg\"},{\"url\":\"fmn057/20111221/1130/original_kJoO_05d9000251de125c.jpg\"}]}";
	using namespace boost::property_tree;

	std::stringstream ss(str);
	ptree pt;
	try{    
		read_json(ss, pt);
	}
	catch(ptree_error & e) {
		return 1; 
	}

	// �޸�/����һ��key-value��key������������
	pt.put("upid", "00001");

	// ����һ������
	ptree exif_array;
	ptree array1, array2, array3;
	array1.put("Make", "NIKON");
	array2.put("DateTime", "2011:05:31 06:47:09");
	array3.put("Software", "Ver.1.01");
	exif_array.push_back(std::make_pair("", array1));
	exif_array.push_back(std::make_pair("", array2));
	exif_array.push_back(std::make_pair("", array3));

	//   exif_array.push_back(std::make_pair("Make", "NIKON"));
	//   exif_array.push_back(std::make_pair("DateTime", "2011:05:31 06:47:09"));
	//   exif_array.push_back(std::make_pair("Software", "Ver.1.01"));

	pt.put_child("exifs", exif_array);
	std::stringstream s2;
	write_json(s2, pt);
	std::string outstr = s2.str();

	return 0;
}

int ParseJson()
{
	std::string str = "{\"code\":0,\"images\":[{\"url\":\"fmn057/20111221/1130/head_kJoO_05d9000251de125c.jpg\"},{\"url\":\"fmn057/20111221/1130/original_kJoO_05d9000251de125c.jpg\"}]}";
	using namespace boost::property_tree;

	std::stringstream ss(str);
	ptree pt;
	try{    
		read_json(ss, pt);
	}
	catch(ptree_error & e) {
		return 1; 
	}

	try{
		int code = pt.get<int>("code");   // �õ�"code"��value
		ptree image_array = pt.get_child("images");  // get_child�õ��������

		// ��������
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, image_array)
		{
			std::stringstream s;
			write_json(s, v.second);
			std::string image_item = s.str();
		}
	}
	catch (ptree_error & e)
	{
		return 2;
	}
	return 0;
}