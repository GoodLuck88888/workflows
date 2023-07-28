
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include <iostream>
#include <algorithm> 
#include <vector>
#include <string>
#include <sstream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

std::string encrypt_key = "!xw@202#";

namespace py = pybind11;

py::none validate(py::object self, py::str module_name);

class RegInfo {

public:
    int user_count;
    std::string module_name;
    std::string customer_name;
    
    std::string expire_date;
    std::string machine_code;

    RegInfo() {
        user_count = 0;
        expire_date = "";
        machine_code = "";
        module_name = "";
        customer_name = "";
    }

    RegInfo(
        std::string machine_code, 
        std::string module_name, 
        std::string customer_name, 
        int user_count,
        std::string expire_date) 
    {
        this->machine_code = machine_code;
        this->expire_date = expire_date;
        this->user_count = user_count;
        this->module_name = module_name;
        this->customer_name = customer_name;
    }

    bool check_valid(std::string machine_code, int user_count, std::string expire_date) {

        if (machine_code != this->machine_code) {
            throw std::runtime_error("Invalid machine code, input machine code:" + machine_code + " current machine code:" + this->machine_code);
        }

        if (this->user_count != 0 && user_count > this->user_count) {
            throw std::runtime_error("Invalid user count");
        }

        if (expire_date > this->expire_date) {
            throw std::runtime_error("Your registration code has expired");
        }

        return true;
    }
};

/**
 * get system info use python
*/
std::string get_system_info() {

    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");

    // get system info
    pybind11::object system_info = sys.attr("platform");

    // convert to string
    std::string system_info_str = system_info.cast<std::string>();

    // convert to lower case
    std::transform(system_info_str.begin(), system_info_str.end(), system_info_str.begin(), ::tolower);

    // replace space with underscore
    std::replace(system_info_str.begin(), system_info_str.end(), ' ', '_');
    return system_info_str;
}

/**
 * check is windows
*/
bool is_windows() {

    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");

    pybind11::object is_windows = sys.attr("platform")
        .attr("startswith")("win");
    return is_windows.cast<bool>();
}

/**
 * check is mac
*/
bool is_mac() {

    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");

    pybind11::object is_mac = sys.attr("platform")
        .attr("startswith")("darwin");
    return is_mac.cast<bool>();
}

/**
 * check is linux
*/
bool is_linux() {

    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");

    pybind11::object is_linux = sys.attr("platform")
        .attr("startswith")("linux");
    return is_linux.cast<bool>();
}

/**
 * get disk info, use python
*/
std::string get_disk_info() {

    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");
    pybind11::module shutil = pybind11::module::import("shutil");

    // Get disk usage
    pybind11::object disk_usage = shutil.attr("disk_usage")("/");

    // Get total disk size
    pybind11::object total_disk_size = disk_usage.attr("total");

    // Convert to GB
    int total_disk_size_gb = pybind11::cast<double>(total_disk_size) / (1024 * 1024 * 1024);

    // Convert to string
    std::stringstream ss;
    ss << total_disk_size_gb;
    std::string total_disk_size_gb_str = ss.str();

    // Concatenate disk size and unit
    std::string disk_info_str = total_disk_size_gb_str + " GB";

    return disk_info_str;
}

/**
 * get memory info, use python
*/
std::string get_memory_info() {
    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module psutil = pybind11::module::import("psutil");

    // Get memory info
    pybind11::object virtual_memory = psutil.attr("virtual_memory")();
    pybind11::object total_memory = virtual_memory.attr("total");

    // Convert to GB
    long long memory_size_gb = pybind11::cast<long long>(total_memory) / (1024 * 1024 * 1024);

    // Convert to string
    std::stringstream ss;
    ss << memory_size_gb;
    std::string memory_size_gb_str = ss.str();

    // Concatenate memory size and unit
    std::string memory_info_str = memory_size_gb_str + " GB";

    return memory_info_str;
}

/**
 * get CPU info, use python
*/
std::string get_cpu_info() {
    
    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");
    pybind11::module psutil = pybind11::module::import("psutil");

    // Get CPU info
    pybind11::object cpu_info = psutil.attr("cpu_count")();

    // Convert to string
    std::string cpu_info_str = std::to_string(pybind11::cast<int>(cpu_info));

    // Concatenate CPU number and unit
    cpu_info_str = cpu_info_str + " Core";

    return cpu_info_str;
}

/**
 * get all system info
*/
std::string get_all_system_info() {

    std::string os_info = get_system_info();
    std::string disk_info = get_disk_info();
    std::string memory_info = get_memory_info();
    std::string cpu_info = get_cpu_info();

    // concatenate all info
    std::string all_system_info = os_info + "\n" + disk_info + "\n" + memory_info + "\n" + cpu_info;
    return all_system_info;
}

/**
 * get machine code
*/
std::string get_machine_code() {

    // pybind11::scoped_interpreter guard{};

    std::string all_system_info = get_all_system_info();

    // convert to char array
    char* all_system_info_char = new char[all_system_info.length() + 1];
    strcpy(all_system_info_char, all_system_info.c_str());

    // generate machine code
    std::string machine_code = std::to_string(std::hash<std::string>{}(all_system_info_char));

    // ensure machine code length is 24
    machine_code = std::string(24 - machine_code.length(), '0') + machine_code;

    // transform machine code to xxxx-xxxs-xxxx-xxxx-xxxx and check the length
    machine_code = machine_code.substr(0, 4) + "-" + machine_code.substr(4, 4) + "-" + machine_code.substr(8, 4) + "-" + machine_code.substr(12, 4) + "-" + machine_code.substr(16, 4);

    return machine_code;
}

/**
 * des encrypt
*/
std::string des_encrypt(
    const std::string& machine_code, 
    const std::string& module_name,  
    const std::string& customer_name,
    int user_count, 
    const std::string& expire_date) 
{
    // use python to encrypt the device info
    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");
    pybind11::module base64 = pybind11::module::import("base64");
    pybind11::module des = pybind11::module::import("Crypto.Cipher.DES");
    pybind11::module hashlib = pybind11::module::import("hashlib");
    pybind11::module padding = pybind11::module::import("Crypto.Util.Padding");

    pybind11::object pad = pybind11::module::import("Crypto.Util.Padding").attr("pad");
    pybind11::object unpad = pybind11::module::import("Crypto.Util.Padding").attr("unpad");

    pybind11::str encrypt_key_str = pybind11::str(encrypt_key);
    pybind11::bytes encrypt_key_bytes = encrypt_key_str.attr("encode")("utf-8");
    pybind11::object key = hashlib.attr("md5")(encrypt_key_bytes).attr("digest")();
    std::string key_str = pybind11::str(key).cast<std::string>();
    key_str = key_str.substr(0, 8);
    pybind11::bytes key_bytes = pybind11::bytes(key_str);
    pybind11::object cipher = des.attr("new")(key_bytes, des.attr("MODE_ECB"));

    std::string total_string = machine_code + '/' + module_name + '/' + customer_name + '/' + std::to_string(user_count) + '/' + expire_date;
    pybind11::bytes total_string_bytes = pybind11::bytes(total_string);
    pybind11::object encrypt_device_info = cipher.attr("encrypt")(pad(total_string_bytes, 8, "pkcs7"));
    pybind11::object encrypt_device_info_base64 = base64.attr("b64encode")(encrypt_device_info);

    pybind11::str registrationCode = pybind11::str(encrypt_device_info_base64.attr("decode")("utf-8"));

    return registrationCode;
}

/**
 * decrypt the registration code
*/
RegInfo des_decrypt(const std::string& registration_code) {

    // use python to encrypt the device info
    pybind11::module sys = pybind11::module::import("sys");
    pybind11::module os = pybind11::module::import("os");
    pybind11::module base64 = pybind11::module::import("base64");
    pybind11::module des = pybind11::module::import("Crypto.Cipher.DES");
    pybind11::module hashlib = pybind11::module::import("hashlib");
    pybind11::module padding = pybind11::module::import("Crypto.Util.Padding");

    pybind11::object pad = pybind11::module::import("Crypto.Util.Padding").attr("pad");
    pybind11::object unpad = pybind11::module::import("Crypto.Util.Padding").attr("unpad");

    pybind11::str encrypt_key_str = pybind11::str(encrypt_key);
    pybind11::bytes encrypt_key_bytes = encrypt_key_str.attr("encode")("utf-8");
    pybind11::object key = hashlib.attr("md5")(encrypt_key_bytes).attr("digest")();
    std::string key_str = pybind11::str(key).cast<std::string>();
    key_str = key_str.substr(0, 8);
    pybind11::bytes key_bytes = pybind11::bytes(key_str);
    pybind11::object cipher = des.attr("new")(key_bytes, des.attr("MODE_ECB"));

    pybind11::bytes total_string_bytes = pybind11::bytes(registration_code);

    // decode to string
    pybind11::object total_string_obj = cipher.attr("decrypt")(
        base64.attr("b64decode")(total_string_bytes)).attr("decode")("utf-8");
    std::string total_string = pybind11::str(total_string_obj);

    RegInfo reg_info;
    std::stringstream ss(total_string);
    std::string item;
    std::vector<std::string> total_string_vec;
    while (std::getline(ss, item, '/')) {
        total_string_vec.push_back(item);
    }

    reg_info.machine_code = total_string_vec[0];
    reg_info.module_name = total_string_vec[1];
    reg_info.customer_name = total_string_vec[2];
    reg_info.user_count = std::stoi(total_string_vec[3]);
    reg_info.expire_date = total_string_vec[4];

    return reg_info;
}

std::string decode_machine_code(const std::string& registration_code) {
    RegInfo reg_info;
    reg_info = des_decrypt(registration_code);
    return reg_info.machine_code;
}

/**
 * generate the registration code
*/
std::string gen_reg_code(
    const std::string& machineCode, 
    const std::string& moudleName,
    const std::string& customerName,
    int userCount, 
    const std::string& expireDate) 
{
    std::string registrationCode = des_encrypt(
        machineCode, moudleName, customerName, userCount, expireDate);
    return registrationCode;
}

/**
 * verify the registration code
*/
bool verify_reg_code(
    const std::string& registrationCode, int user_count, const std::string& today) {

    // get the machine code
    std::string machineCode = get_machine_code();

    // decrypt the registration code
    RegInfo regInfo = des_decrypt(registrationCode);
    return regInfo.check_valid(machineCode, user_count, today);
}

/**
 * get today
*/
std::string get_today() {

    // get current date
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::string today = std::to_string(
        1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday);
    
    return today;
}

/**
 * register the module
*/
bool reg_module(py::object& context) {

    py::dict locals;

    locals["self"] = context["self"];
    locals["module"] = context["module"];
    locals["fields"] = context["fields"];

    locals["regCode"] = context["regCode"];
    locals["machineCode"] = context["machineCode"];
    locals["userCount"] = context["userCount"];
    locals["xw_cbase"] = context["xw_cbase"];

    RegInfo regInfo = des_decrypt(locals["regCode"].cast<std::string>());
    py::str expireDate = regInfo.expire_date;
    locals["expireDate"] = expireDate;

    // write the registration code to the database
    py::exec(R"(
        module.write({
            'reg_code': regCode,
            'reg_date': fields.Date.today(),
            'machine_code': machineCode,
            'user_count': userCount,
            'expire_date': expireDate
        })
    )", locals);

    // check the registration code
    if (!validate(locals["self"], regInfo.module_name)) {
        return false;
    }

    return true;
}

/**
 * setup the module fields
*/
py::none setup_moudle_fields(py::object context) {
    py::dict locals;

    locals["attrs"] = context["attrs"];
    locals["models"] = context["models"];
    locals["fields"] = context["fields"];
    locals["api"] = context["api"];
    
    py::exec(R"(
        reg_code = fields.Char(string="Reg Code", required=False)
        attrs['reg_code'] = reg_code

        reg_date = fields.Date(string="Reg Date", required=False)
        attrs['reg_date'] = reg_date

        machine_code = fields.Char(string="Machine Code", required=False)
        attrs['machine_code'] = machine_code

        user_count = fields.Integer(string="User Count", required=False)
        attrs['user_count'] = user_count

        expire_date = fields.Date(string="Expire Date", required=False)
        attrs['expire_date'] = expire_date

    )", locals);

    return py::none();
}

/**
 * this is a example for setup the module methods
*/
py::none setup_moudle_methods(py::object context) {
    py::dict locals;

    locals["cls"] = context["cls"];
    locals["attrs"] = context["attrs"];
    locals["models"] = context["models"];
    locals["fields"] = context["fields"];
    locals["api"] = context["api"];
    
    py::exec(R"(
        @api.depends('task_define')
        def setup_moudle_methods(self):
            """
            compute is engine task
            :return:
            """
            for record in self:
                record.is_engine_task = (
                    not hasattr(
                        record.task_define, 'is_engine_task') or record.task_define.is_engine_task())
        setattr(cls, 'setup_moudle_methods', setup_moudle_methods)
    )", locals);
    
    return py::none();
}

/**
 * setup the demo fields
*/
py::none setup_demo_fields(py::object context) {
    py::dict locals;

    locals["attrs"] = context["attrs"];
    locals["models"] = context["models"];
    locals["fields"] = context["fields"];
    locals["api"] = context["api"];
    
    py::exec(R"(
        name = fields.Char()
        attrs['name'] = name

        value = fields.Integer()
        attrs['value'] = value

        value2 = fields.Float(compute="_value_pc", store=True)
        attrs['value2'] = value2

        description = fields.Text()
        attrs['description'] = description
    )", locals);

    return py::none();
}

/**
 * setup the demo methods
*/
py::none setup_demo_methods(py::object context) {
    py::dict locals;

    locals["attrs"] = context["attrs"];
    locals["models"] = context["models"];
    locals["fields"] = context["fields"];
    locals["api"] = context["api"];
    locals["exceptions"] = context["exceptions"];
    locals["xw_cbase"] = context["xw_cbase"];
    locals["cls"] = context["cls"];

    // 也可以在这里直接调用函数验证，如果是在python中运行的话则需要调用xc_cbase.validate
    
    py::exec(R"(
        
        @api.depends('value')
        def _value_pc(self):
            for record in self:
                record.value2 = float(record.value) / 100
        setattr(cls, '_value_pc', _value_pc)

        def test_raise(self):
            if xw_cbase.validate(self, 'xw_auth_demo'):
                raise exceptions.ValidationError('Please regist the module!')
            else:
                raise exceptions.ValidationError('That is ok!')
        setattr(cls, 'test_raise', test_raise)

    )", locals);

    return py::none();
}

/**
 * patch the MetaModel
*/
py::none patch_meta_model(py::object context) {
    py::dict locals;

    locals["MetaModel"] = context["MetaModel"];

    py::exec(R"(
        # patch the _new method of MetaModel
        _old_new = MetaModel.__new__
        def _new(cls, name, bases, attrs):

            if attrs.get('dynamic_init_fields', None) is not None:
                attrs['dynamic_init_fields'](attrs)

            # create the class
            new_class = _old_new(cls, name, bases, attrs)

            # call the dynamic init method  
            if attrs.get('dynamic_init_method', None) is not None:
                attrs['dynamic_init_method'](new_class, attrs)

            # return the class
            return new_class

        # patch the __new__ method of MetaModel
        MetaModel.__new__ = _new
    )", locals);
    
    return py::none();
}

/**
 * validate the module
*/
py::none validate(py::object self, py::str module_name) {
    py::dict locals;

    locals["self"] = self;
    locals["module_name"] = module_name;

    // query the registration code
    py::exec(R"(
        module = self.env['ir.module.module'].sudo().search([('name', '=', module_name)])
        regCode = module.reg_code
    )", locals); 

    std::string regCode = locals["regCode"].cast<std::string>();

    // get user count
    py::exec(R"(
        user_count = self.env['res.users'].sudo().search_count([])
    )", locals);

    std::string today = get_today();

    // get today   
    int userCount = locals["user_count"].cast<int>();

    // check the registration code
    if (!verify_reg_code(regCode, userCount, today)) {
        throw std::runtime_error("Invalid registration code");
    }

    return py::none();
}

PYBIND11_MODULE(xw_cbase, m) {

    m.doc() = R"pbdoc(
        xw_cbase module
        -----------------------
        .. currentmodule:: 
        .. autosummary::
           :toctree: _generate
           set reg module
    )pbdoc";

    m.def("gen_reg_code", &gen_reg_code, R"pbdoc(
        generate registration code
    )pbdoc");

    m.def("verify_reg_code", &verify_reg_code, R"pbdoc(
        verify registration code
    )pbdoc");

    m.def("setup_moudle_fields", &setup_moudle_fields, R"pbdoc(
        set up moudle fields
    )pbdoc");

    m.def("get_machine_code", &get_machine_code, R"pbdoc(
        generate machine code
    )pbdoc");

    m.def("patch_meta_model", &patch_meta_model, R"pbdoc(
        patch_meta_model
    )pbdoc");

    m.def("decode_machine_code", &decode_machine_code, R"pbdoc(
        decode machine code
    )pbdoc");

    // setup_demo_fields
    m.def("setup_demo_fields", &setup_demo_fields, R"pbdoc(
        setup demo fields
    )pbdoc");

    // validate
    m.def("validate", &validate, R"pbdoc(
        validate
    )pbdoc");

    // setup_demo_methods
    m.def("setup_demo_methods", &setup_demo_methods, R"pbdoc(
        setup demo methods
    )pbdoc");

    m.def("reg_module", &reg_module, R"pbdoc(
        reg module
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif

}
