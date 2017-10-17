#include <phpcpp.h>
#include <iostream>
#include <unordered_map>

template <typename t1, typename t2>
class HashMap {

private:
	std::unordered_map<t1, t2> map;

public:
	void insert(const t1 key, const t2 value) {
		map[key] = value;
	}

	int exists(const t1 key) {

		typename std::unordered_map<t1, t2>::const_iterator search = map.find(key);
		if (search == map.end()) {
			return false;
		}
		return true;

	}

	t2 get(t1 key) {
		return map[key];
	}

};

Php::Value cacheControl(Php::Parameters &param) {

	static HashMap<std::string, std::string> serializedData;

	std::string key = param[0].buffer(), value = "";
	if (param.size() == 2) {
		value = param[1].buffer();
	}

	if (serializedData.exists(key)) {

		if (value != "") {
			serializedData.insert(key, value);
		}

	}
	else {
		serializedData.insert(key, value);
	}

	return serializedData.get(key);

}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

	/**
	*  Function that is called by PHP right after the PHP process
	*  has started, and that returns an address of an internal PHP
	*  strucure with all the details and features of your extension
	*
	*  @return void*   a pointer to an address that is understood by PHP
	*/
	PHPCPP_EXPORT void *get_module()
	{
		// static(!) Php::Extension object that should stay in memory
		// for the entire duration of the process (that's why it's static)
		static Php::Extension extension("Agility", "0.1");
		extension.add<cacheControl>("cacheControl", {
			Php::ByVal("key", Php::Type::String, true),
			Php::ByVal("value", Php::Type::String, false)
		});

		// @todo    add your own functions, classes, namespaces to the extension

		// return the extension
		return extension;
	}
}
