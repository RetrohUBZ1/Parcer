#include <string>
#include <cctype>
#include <sstream>
#include <stack>
#include <map>
#include <iostream>
#include <algorithm>
#include "val.h"
using namespace std;



Value Value::operator*(const Value& op) const
{
    /*if (this->IsInt() && op.IsInt())
    {
        return Value(this->GetInt() * op.GetInt());
    }

    else if (this->IsReal() && op.IsReal())
    {
        return Value(this->GetReal() * op.GetReal());
    }

    else if ((this->IsInt() && op.IsReal()) || (this->IsReal() && op.IsInt()))
    {
        return Value(this->GetReal() * op.GetReal());
    }

    else if (this->IsString() && op.IsInt())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = std::stod(str) * op.GetInt();
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

       // return Value(result);
    }
    
    else if (this->IsInt() && op.IsString())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = this->GetInt() * std::stod(str);
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        //return Value(result);
    }


    else if (this->IsReal() && op.IsString())
    {
        std::string str = op.GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = this->GetReal() * std::stod(str);
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }

        //return Value(result);
    }
    
    else if (this->IsString() && op.IsReal())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = std::stod(str) * op.GetReal();
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }
    
    }
    try
    {
            if (this->IsInt() && op.IsInt())
        {
            return Value(this->GetInt() * op.GetInt());
        }

        else if (this->IsReal() && op.IsReal())
        {
            return Value(this->GetReal() * op.GetReal());
        }

        else if ((this->IsInt() && op.IsReal()))
        {
            return Value(this->GetInt() * op.GetReal());
        }

        else if((this->IsReal() && op.IsInt()))
        {
        return Value(this->GetReal() * op.GetInt());
        }

        else if(this->IsString() && op.IsInt())
        {
        return Value(stod(this->GetString()) * op.GetInt());
        }

        else if(this->IsString() && op.IsReal())
        {
        return Value(stod(this->GetString()) * op.GetReal());
        }

        else if(this->IsInt() && op.IsString())
        {
        return Value(stod(this->GetString()) * op.GetReal());
        }
        
        else if(this->IsString() && op.IsString())
        {
        return Value(stod(this->GetString()) * stod(op.GetString()));
        }

        else if(this->IsReal() && op.IsString())
        {
        return Value(stod(this->GetString()) * op.GetReal());
        }
        
    }
    catch(invalid_argument & arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    return Value();*/
    {
    // Local Declaration
    double oper1, oper2;

    if (this->IsString() && op.IsString()) // Both operands are strings
    {
        //both operands must be converted to a numeric
        try
        {
            oper1 = stod(this->GetString()); //may throw invalid_argument()
            oper2 = stod(op.GetString()); //may throw invalid_argument()

        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        return Value(oper1 * oper2);
    }
    else if (this->IsReal() && op.IsString()) // Oper1 is double and Oper2 is string
    {
        //second operand must be converted to a numeric
        try
        {
            oper2 = stod(op.GetString()); //may throw invalid_argument()
        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        oper1 = this->GetReal();
        return Value(oper1 * oper2);
    }
    else if (this->IsString() && op.IsReal()) // Oper1 is string and Oper2 is double
    {
        //first operand must be converted to a numeric
        try
        {
            oper1 = stod(this->GetString()); //may throw invalid_argument()
        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        oper2 = op.GetReal();
        return Value(oper1 * oper2);
    }
    else if (this->IsErr() || op.IsErr()) // Checking if either operator is an error
    {
        return Value();
    }
    else if (this->IsReal() && op.IsReal()) // Both operators are doubles
    {
        oper1 = this->GetReal();
        oper2 = op.GetReal();
        return Value(oper1 * oper2);
    }

    return Value();
}
}
/*
Value Value::operator*(const Value& op) const
{
    if (IsInt() && op.IsInt()) {
        return Value(GetInt() * op.GetInt());
    } else if (IsReal() && op.IsReal()) {
        return Value(GetReal() * op.GetReal());
    } else if ((IsInt() && op.IsReal()) || (IsReal() && op.IsInt())) {
        return Value(GetReal() * op.GetReal());
    } else if (IsString() && op.IsInt()) {
        string result = "";
        for (int i = 0; i < op.GetInt(); i++) {
            result += GetString();
        }
        return Value(result);
    } else if (IsInt() && op.IsString()) {
        string result = "";
        for (int i = 0; i < GetInt(); i++) {
            result += op.GetString();
        }
        return Value(result);
    } else {
        throw runtime_error("ERROR: invalid operands for operator*");
    }
}
*/

/*
Value operator*(const Value& op)
{

}
*/
/*Value Value::operator==(const Value& op) const
{
    if (this->T != op.T)
    {
        return Value();
    }

    switch (T)
    {
    case VINT:
        return Value(this->Itemp == op.Itemp);
    case VREAL:
        return Value(this->Rtemp == op.Rtemp);
    case VSTRING:
        try {
            double thisVal = std::stod(this->Stemp);
            double opVal = std::stod(op.Stemp);
            return Value(thisVal == opVal);
        }
        catch (const std::invalid_argument& e) {
            return Value("Invalid conversion from string to double.");
        }
        catch (const std::out_of_range& e) {
            return Value("Number out of range.");
        }
    case VBOOL:
        return Value(this->Btemp == op.Btemp);
    default:
        return Value();
    }
}









Value Catenate(const Value& oper)
{

}
*/

/*Value Value::Catenate(const Value& oper) const {
    if (this->IsString() && oper.IsString()) {
        string str = Stemp + oper.GetString();
        return Value(str);
    } else if (this->IsNumeric() && oper.IsNumeric()) {
        try {
            double result = this->GetNumeric() + oper.GetNumeric();
            return Value(to_string(result));
        } catch (const std::exception& e) {
            throw std::runtime_error("RUNTIME ERROR: Catenation failed with error: " + std::string(e.what()));
        }
    } else {
        throw std::runtime_error("RUNTIME ERROR: Catenation failed - both operands must be either strings or numeric values.");
    }
}*/


/*Value Value::Catenate(const Value& oper) const
{
    if (this->IsString() && oper.IsString())
    {
        string str1 = this->GetString();
        string str2 = oper.GetString();

        // check if str1 contains only digits
        bool str1_all_digits = true;
        for (char c : str1) {
            if (!isdigit(c) && c != '.') {
                str1_all_digits = false;
                break;
            }Re
        }

        // check if str2 contains only digits
        bool str2_all_digits = true;
        for (char c : str2) {
            if (!isdigit(c) && c != '.') {
                str2_all_digits = false;
                break;
            }
        }

        // if both strings contain only digits, concatenate them as doubles
        if (str1_all_digits && str2_all_digits) {
            double val1 = stod(str1);
            double val2 = stod(str2);
            double result = val1 + val2;
            return Value(to_string(result));
        }

        // otherwise, concatenate the strings as is
        else {
            string str = str1 + str2;
            return Value(str);
        }
    }
    else
    {
        throw runtime_error("Catenate: both operands must be strings.");
    }
}*/

/*Value Value::Catenate(const Value& oper) const
{
if (this->)
}*/

/*Value Value::Catenate(const Value& oper) const
{
    if (IsString() && oper.IsString())
    {
        string str1 = Stemp;
        string str2 = oper.GetString();
        string result = "";

        // Check if both strings contain numbers
        bool has_number1 = any_of(str1.begin(), str1.end(), ::isdigit);
        bool has_number2 = any_of(str2.begin(), str2.end(), ::isdigit);

        if (has_number1 && has_number2)
        {
            // Both strings contain numbers, convert to double and add them
            double num1 = stod(str1);
            double num2 = stod(str2);
            result = to_string(num1 + num2);
        }

        else
        {
            // At least one string does not contain a number, concatenate as usual
            result = str1 + str2;
        }
        return Value(result);
    }

    else
    {
        throw runtime_error("Catenate: both operands must be strings.");
    }
}*/



Value Value::Catenate(const Value& oper) const
{
    if (this->IsString() && oper.IsString()) {
        string str = Stemp + oper.GetString();
        return Value(str);
    }
    else if (IsReal() && oper.IsReal()) {
        string str1 = to_string(GetReal());
        str1 = str1.substr(0, str1.find('.') + 3);
        string str2 = to_string(oper.GetReal());
        str2 = str2.substr(0, str2.find('.') + 3);
        double val1 = stod(str1);
        double val2 = stod(str2);
        string str = (val1 >= val2) ? str1.append(str2) : str2.append(str1);
        return Value(str);
    }
    else if (IsReal() && oper.IsString()) {
        string str1 = to_string(GetReal());
        str1 = str1.substr(0, str1.find('.') + 3);
        return Value(oper.GetString().append(str1));
    }
    else if (IsString() && oper.IsReal()) {
        string str1 = this->GetString();
        string str2 = to_string(oper.GetReal());
        str2 = str2.substr(0, str2.find('.') + 3);
        return Value(str1.append(str2));
    }

    else if (IsErr() || oper.IsErr())
    {
        return Value();
    }

    return Value();
}


/*
Value operator^(const Value& oper)
{

}
*/


Value Value::operator^(const Value& oper) const {
    /*if (this->IsInt() && oper.IsInt())
    {
        int result = pow(this->Itemp, oper.Itemp);
        return Value(result);
    }*/

    if (this->IsReal() && oper.IsReal())
    {
        double result = pow(this->Rtemp, oper.Rtemp);
        return Value(result);
    }

    /*else if (this->IsInt() && oper.IsReal())
    {
        double result = pow(this->Itemp, oper.Rtemp);
        return Value(result);
    }

    else if (this->IsReal() && oper.IsInt())
    {
        double result = pow(this->Rtemp, oper.Itemp);
        return Value(result);
    }*/

    else
    {
        return Value();
    }
}


/*
Value SLthan(const Value& oper)
{

}

Value Value::SLthan(const Value& oper) const
{
    if (this->IsErr() || oper.IsErr())
    {
        return Value();
    }
    else if (IsString() && oper.IsString())
    {
        return Value(GetString() < oper.GetString());
    }
    else if (IsString() && oper.IsReal())
    {
        double real_val = oper.GetReal();
        string real_str = to_string(real_val);
        size_t dot_pos = real_str.find('.');
        if (dot_pos != string::npos && dot_pos < real_str.length() - 1) {
            // remove trailing zeros
            real_str.erase(real_str.find_last_not_of('0') + 1, string::npos);
        }
        return Value(GetString() < real_str);
    }
    else if (IsReal() && oper.IsString())
    {
        double this_real_val = this->GetReal();
        string this_real_str = to_string(this_real_val);
        size_t dot_pos = this_real_str.find('.');
        if (dot_pos != string::npos && dot_pos < this_real_str.length() - 1) {
            // remove trailing zeros
            this_real_str.erase(this_real_str.find_last_not_of('0') + 1, string::npos);
        }
        return Value(this_real_str < oper.GetString());
    }
    else if (IsReal() && oper.IsReal())
    {
        double this_real_val = this->GetReal();
        double oper_real_val = oper.GetReal();
        string this_real_str = to_string(this_real_val);
        string oper_real_str = to_string(oper_real_val);
        size_t this_dot_pos = this_real_str.find('.');
        if (this_dot_pos != string::npos && this_dot_pos < this_real_str.length() - 1) {
            // remove trailing zeros
            this_real_str.erase(this_real_str.find_last_not_of('0') + 1, string::npos);
        }
        size_t oper_dot_pos = oper_real_str.find('.');
        if (oper_dot_pos != string::npos && oper_dot_pos < oper_real_str.length() - 1) {
            // remove trailing zeros
            oper_real_str.erase(oper_real_str.find_last_not_of('0') + 1, string::npos);
        }
        return Value(this_real_str < oper_real_str);
    }

    return Value();
}
*/
//numeric overloaded less than operator of this with op
Value Value::operator<(const Value& oper) const
{
  /*  try
    {
    if(IsInt() && oper.IsInt()) 
    {
        return Value(GetInt() > oper.GetInt());
    } 
    
    else if (IsReal() && oper.IsReal()) {
        return Value(GetReal() > oper.GetReal());
    } 
    
    else if (IsInt() && oper.IsReal()) 
    {
        return Value(static_cast<double>(GetInt()) > oper.GetReal());
    } 
    
    else if (IsReal() && oper.IsInt()) 
    {
        return Value(GetReal() > static_cast<double>(oper.GetInt()));
    } 
    
    else if(IsString() && oper.IsInt())
      {
      return Value(stoi(GetString())>oper.GetInt());
      }
    else if(IsString() && oper.IsReal())
      {
      return Value(stod(GetString())>oper.GetInt());
      }
    else if(IsInt() && oper.IsString())
      {
      return Value(GetInt()>stoi(oper.GetString()));
      }
    else if(IsReal() && oper.IsString())
      {
      return Value(GetInt()>stod(oper.GetString()));
      }
    }
    catch(invalid_argument & arg)
    {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
    }
    return Value();*/
    {
    // Local Declaration
    double oper1, oper2;

    if (this->IsString() && oper.IsString()) // Both operands are strings
    {
        //both operands must be converted to a numeric
        try
        {
            oper1 = stod(this->GetString()); //may throw invalid_argument()
            oper2 = stod(oper.GetString()); //may throw invalid_argument()

        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        return Value(oper1 < oper2);
    }
    else if (this->IsReal() && oper.IsString()) // Oper1 is double and Oper2 is string
    {
        //second operand must be converted to a numeric
        try
        {
            oper2 = stod(oper.GetString()); //may throw invalid_argument()
        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        oper1 = this->GetReal();
        return Value(oper1 < oper2);
    }
    else if (this->IsString() && oper.IsReal()) 
    {
        
        try
        {
            oper1 = stod(this->GetString()); //may throw invalid_argument()
        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        oper2 = oper.GetReal();
        return Value(oper1 < oper2);
    }
    else if (this->IsErr() || oper.IsErr()) // Checking if either operator is an error
    {
        return Value();
    }
    else if (this->IsReal() && oper.IsReal()) // Both operators are doubles
    {
        oper1 = this->GetReal();
        oper2 = oper.GetReal();
        return Value(oper1 < oper2);
    }

    return Value();
}
}

//numeric overloaded greater than operator of this with op
Value Value::operator>(const Value& op) const
{
    try
    {
        if(IsInt() && op.IsInt()) 
        {
            return Value(GetInt() > op.GetInt());
        } 

        else if (IsReal() && op.IsReal()) {
            return Value(GetReal() > op.GetReal());
        } 

        else if (IsInt() && op.IsReal()) 
        {
            return Value(static_cast<double>(GetInt()) > op.GetReal());
        } 

        else if (IsReal() && op.IsInt()) 
        {
            return Value(GetReal() > static_cast<double>(op.GetInt()));
        } 

        else if(IsString() && op.IsInt())
        {
          return Value(stoi(GetString())>GetInt());
        }
        else if(IsString() && op.IsReal())
          {
          return Value(stod(GetString())>GetInt());
          }
        else if(IsInt() && op.IsString())
          {
          return Value(GetInt()>stoi(GetString()));
          }
        else if(IsReal() && op.IsString())
          {
          return Value(GetInt()>stod(GetString()));
          }
    }
    catch(invalid_argument & arg)
    {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
    }/*
        
    else 
    {
        cout<< "Invalid conversion from string to number."<<endl;
        return Value();
    }
    return Value();*/
    
    return Value();
}

//numeric overloaded equality operator of this with op
  Value Value::operator==(const Value& op) const
  { 
  /*{
  if (m_type != op.m_type) 
  {
    // Types are different, so they can't be equal
    return Value(false);
  } 
      else 
  {*/
    // Types are the same, so compare their values
   //switch (m_type) 
      try {
        if(this->IsInt() && op.IsInt())
        {
           return Value(this->GetInt() == op.GetInt());
        }
      else if(this->IsReal() && op.IsReal())
        {
            return Value(this->GetReal() == op.GetReal());
        }
      else if(this->IsInt() && op.IsReal())
        {
            return Value(this->GetInt() == op.GetReal());
        }
      else if(this->IsReal() && op.IsInt())
        {
            return Value(this->GetReal() == op.GetInt());
        }
      
      /*else if(this->IsBool() && op.IsBool())
        {
            return Value(this->GetBool() == op.GetBool());
        }
      else if(this->IsString() && op.IsString())
          {
        return Value(this->GetString() == op.GetString());
          }
      else{
        // Invalid type, so they can't be equal
        return Value();
        }*/
      }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
      
      return Value();
    }


// numeric overloaded add op to this
Value Value::operator+(const Value& op) const
{
    /*if (this->IsInt() && op.IsInt())
    {
        return Value(this->GetInt() + op.GetInt());
    }

    else if (this->IsReal() && op.IsReal())
    {
        return Value(this->GetReal() + op.GetReal());
    }

    else if ((this->IsInt() && op.IsReal()) || (this->IsReal() && op.IsInt()))
    {
        return Value(this->GetReal() + op.GetReal());
    }

    else if (this->IsString() && op.IsInt())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = std::stod(str) + op.GetInt();
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

       // return Value(result);
    }
    
    else if (this->IsInt() && op.IsString())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = this->GetInt() + std::stod(str);
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        //return Value(result);
    }


    else if (this->IsReal() && op.IsString())
    {
        std::string str = op.GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = this->GetReal() + std::stod(str);
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }

        //return Value(result);
    }
    
    else if (this->IsString() && op.IsReal())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = std::stod(str) + op.GetReal();
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }
    
    }*/
        try
    {
        if (this->IsInt() && op.IsInt())
        {
            return Value(this->GetInt() + op.GetInt());
        }

        else if (this->IsReal() && op.IsReal())
        {
            return Value(this->GetReal() + op.GetReal());
        }

        else if ((this->IsInt() && op.IsReal()))
        {
            return Value(this->GetInt() + op.GetReal());
        }

        else if((this->IsReal() && op.IsInt()))
        {
        return Value(this->GetReal() + op.GetInt());
        }

        else if(this->IsString() && op.IsInt())
        {
        return Value(stod(this->GetString()) + op.GetInt());
        }

        else if(this->IsString() && op.IsReal())
        {
        return Value(stod(this->GetString()) + op.GetReal());
        }
            
        else if(this->IsString() && op.IsString())
        {
            return Value(stod(this->GetString()) + stod(op.GetString()));
        }

        else if(this->IsInt() && op.IsString())
        {
        return Value(op.GetReal()+stod(this->GetString()));
        }

        else if(this->IsReal() && op.IsString())
        {
        return Value(op.GetReal()+stod(this->GetString()));
        }
    }
    catch(invalid_argument & arg)
    {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
    }
        
    return Value();
}

// numeric overloaded subtract op from this
Value Value::operator-(const Value& op) const
{
    /*if (this->IsInt() && op.IsInt())
    {
        return Value(this->GetInt() - op.GetInt());
    }

    else if (this->IsReal() && op.IsReal())
    {
        return Value(this->GetReal() - op.GetReal());
    }

    else if ((this->IsInt() && op.IsReal()) || (this->IsReal() && op.IsInt()))
    {
        return Value(this->GetReal() - op.GetReal());
    }

    else if (this->IsString() && op.IsInt())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = std::stod(str) - op.GetInt();
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

       // return Value(result);
    }
    
    else if (this->IsInt() && op.IsString())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = this->GetInt() - std::stod(str);
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        //return Value(result);
    }


    else if (this->IsReal() && op.IsString())
    {
        std::string str = op.GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = this->GetReal() - std::stod(str);
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }

        //return Value(result);
    }
    
    else if (this->IsString() && op.IsReal())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = std::stod(str) - op.GetReal();
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }
    
    }
    try
    {
        if (this->IsInt() && op.IsInt())
        {
            return Value(this->GetInt() - op.GetInt());
        }

        else if (this->IsReal() && op.IsReal())
        {
            return Value(this->GetReal() - op.GetReal());
        }

        else if ((this->IsInt() && op.IsReal()))
        {
            return Value(this->GetInt() - op.GetReal());
        }

        else if((this->IsReal() && op.IsInt()))
        {
        return Value(this->GetReal() - op.GetInt());
        }

        else if(this->IsString() && op.IsInt())
        {
        return Value(stod(this->GetString()) - op.GetInt());
        }

        else if(this->IsString() && op.IsReal())
        {
        return Value(stod(this->GetString()) - op.GetReal());
        }
        
        else if(this->IsString() && op.IsString())
        {
        return Value(stod(this->GetString()) - stod(op.GetString()));
        }

        else if(this->IsInt() && op.IsString())
        {
        return Value(stod(this->GetString()) - op.GetReal());
        }

        else if(this->IsReal() && op.IsString())
        {
        return Value(stod(this->GetString()) - op.GetReal());
        }
    }
    catch(invalid_argument & arg)
    {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
    }
    return Value();*/
    {
    // Local Declaration
    double oper1, oper2;

    if (this->IsString() && op.IsString()) // Both operands are strings
    {
        //both operands must be converted to a numeric
        try
        {
            oper1 = stod(this->GetString()); //may throw invalid_argument()
            oper2 = stod(op.GetString()); //may throw invalid_argument()

        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        return Value(oper1 - oper2);
    }
    else if (this->IsReal() && op.IsString()) // Oper1 is double and Oper2 is string
    {
        //second operand must be converted to a numeric
        try
        {
            oper2 = stod(op.GetString()); //may throw invalid_argument()
        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        oper1 = this->GetReal();
        return Value(oper1 - oper2);
    }
    else if (this->IsString() && op.IsReal()) // Oper1 is string and Oper2 is double
    {
        //first operand must be converted to a numeric
        try
        {
            oper1 = stod(this->GetString()); //may throw invalid_argument()
        }
        catch (invalid_argument& arg)
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        oper2 = op.GetReal();
        return Value(oper1 - oper2);
    }
    else if (this->IsErr() || op.IsErr()) // Checking if either operator is an error
    {
        return Value();
    }
    else if (this->IsReal() && op.IsReal()) // Both operators are doubles
    {
        oper1 = this->GetReal();
        oper2 = op.GetReal();
        return Value(oper1 - oper2);
    }

    return Value();
}
}


// numeric overloaded divide this by op
Value Value::operator/(const Value& op) const
{
    try{
        if (this->IsInt() && op.IsInt())
    {
        return Value(this->GetInt() / op.GetInt());
    }

    else if (this->IsReal() && op.IsReal())
    {
        return Value(this->GetReal() / op.GetReal());
    }

    else if ((this->IsInt() && op.IsReal()))
    {
        return Value(this->GetInt() / op.GetReal());
    }
   
    else if((this->IsReal() && op.IsInt()))
    {
    return Value(this->GetReal() / op.GetInt());
    }
    
    else if(this->IsString() && op.IsInt())
    {
    return Value(stod(this->GetString()) / op.GetInt());
    }
    
    else if(this->IsString() && op.IsReal())
    {
    return Value(stod(this->GetString()) / op.GetReal());
    }
    
    else if(this->IsInt() && op.IsString())
    {
    return Value(op.GetReal()/stod(this->GetString()));
    }
    
    else if(this->IsReal() && op.IsString())
    {
    return Value(op.GetReal()/stod(this->GetString()));
    }
        
    else if(this->IsString() && op.IsString())
    {
        return Value(stod(op.GetString())/stod(this->GetString()));
    }
        
    }
    catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
    
    

    /*else if (this->IsString() && op.IsInt())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = std::stod(str) / op.GetInt();
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

       // return Value(result);
    }
    
    else if (this->IsInt() && op.IsString())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++)
        {
            if (i == 0 && str[i] == '-')
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i]))
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber)
        {
            result = this->GetInt() / std::stod(str);
            return Value(result);
        }
        else
        {
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        //return Value(result);
    }


    else if (this->IsReal() && op.IsString())
    {
        std::string str = op.GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = this->GetReal() / std::stod(str);
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }

        //return Value(result);
    }
    
    else if (this->IsString() && op.IsReal())
    {
        std::string str = this->GetString();
        bool isNumber = true;

        // Check if the string is a number
        for (size_t i = 0; i < str.size(); i++) 
        {
            if (i == 0 && str[i] == '-') 
            {
                continue; // Negative sign is allowed at the beginning
            }
            if (!std::isdigit(str[i])) 
            {
                isNumber = false;
                break;
            }
        }

        double result;
        if (isNumber) 
        {
            result = std::stod(str) / op.GetReal();
            return Value(result);
        } 
        else 
        {
            cout<< "Invalid conversion from string to double."<<endl;
            return Value();
        }
    
    }*/
    return Value();
}

//string equality (-eq) operator of this with op
	Value Value::SEqual(const Value& oper) const
{
    /*if (GetType() != oper.GetType()) {
        return Value(false);
    }*/

    if (this->IsString()) {
        return Value(this->GetString() == oper.GetString());
    }

    return Value(false);
}

//string repetition operation of this with op
//Value Value::Repeat(const Value& oper) const 
//{
    /*if (oper.GetType() != ValueType::INT) {
        throw std::runtime_error("Repeat error: operand is not an integer");
    }*/

    /*int repeatCount = oper.GetInt();

    if (repeatCount < 0) {
        throw std::runtime_error("Repeat error: operand is negative");
    }

    std::string repeatedStr = "";
    for (int i = 0; i < repeatCount; i++) {
        repeatedStr += oper.GetString();
    }

    return Value(repeatedStr);
}*/

//repeat string of this object op number of times
Value Value::Repeat(const Value& op) const {
    //First operands is a string and the second operand is an integer
    double oper;
    if(IsString() && op.IsString()){
        //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value(newstr);
    }
    else if(IsReal() && op.IsString()){
        //Both operands to be converted
        //first operand must be converted to a string
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        for(int i = 1; i <= times; i++){
            newstr += stroper1;
        }
        return Value(newstr);
    }
    else if (IsReal() && op.IsReal()) {
        //Other cases to follow
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        oper = op.GetReal();
        int times = oper;

        for(int i = 1; i <= times; i++){
            newstr += stroper1;
        }
        return Value(newstr);
    }
    else if (IsString() && op.IsReal()){
        oper = op.GetReal();
        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value(newstr);
    }
    else {
        return Value();
    }
} 

//string greater than (-gt) operator of this with op
Value Value::SGthan(const Value& oper) const
{/*
    if (this->IsString() && oper.IsString())
    {
        // Compare the string values of the two values
        std::string str1 = GetString();
        std::string str2 = oper.GetString();
        int len = std::min(str1.length(), str2.length());
        for (int i = 0; i < len; i++) 
        {
            if (str1[i] > str2[i]) 
            {
                return Value(true);
            }
            else if (str1[i] < str2[i]) 
            {
                return Value(false);
            }
        }    
    }
    return Value();*/
    
   ostringstream str1, str2;
    bool status;


    if (this->IsReal() && oper.IsReal()) // Both operands are reals
    {
        //both operands must be converted to strings
        str1 << this->GetReal(); //may throw invalid_argument()
        str2 << oper.GetReal(); //may throw invalid_argument()

        string string1 = str1.str();
        string string2 = str2.str();
        status = string1 > string2;
        return Value(status);
    }
    else if (this->IsReal() && oper.IsString()) // Oper1 is double and Oper2 is string
    {
        //first operand must be converted to a string
        str1 << this->GetReal(); //may throw invalid_argument()

        string string1 = str1.str();
        string string2 = oper.GetString();
        status = string1 > string2;
        return Value(status);
    }
    else if (this->IsString() && oper.IsReal()) // Oper1 is string and Oper2 is double
    {
        //second operand must be converted to a string
        str2 << oper.GetReal(); //may throw invalid_argument()     

        string string1 = this->GetString();
        string string2 = str2.str();
        status = string1 > string2;
        return Value(status);
    }
    else if (this->IsErr() || oper.IsErr()) // Checking if either operator is an error
    {
        return Value();
    }
    else if (this->IsString() && oper.IsString()) // Both operators are strings
    {
        string string1, string2;
        string1 = this->GetString();
        string2 = oper.GetString();
        status = string1 > string2;
        return Value(status);
    }

    return Value();
}

//string less than operator of this with op
	Value Value::SLthan(const Value& oper) const
{
      /* if (this->IsString() && oper.IsString())
    {
        // Compare the string values of the two values
        std::string str1 = GetString();
        std::string str2 = oper.GetString();
        int len = std::min(str1.length(), str2.length());
        for (int i = 0; i < len; i++) 
        {
            if (str1[i] < str2[i]) 
            {
                return Value(true);
            }
            else if (str1[i] > str2[i]) 
            {
                return Value(false);
            }
        }    
    }
    
    return Value();*/
         // Local Declaration
    ostringstream str1, str2;
    bool status;


    if (this->IsReal() && oper.IsReal()) // Both operands are reals
    {
        //both operands must be converted to strings
        str1 << this->GetReal(); //may throw invalid_argument()
        str2 << oper.GetReal(); //may throw invalid_argument()

        string string1 = str1.str();
        string string2 = str2.str();
        status = string1 < string2;
        return Value(status);
    }
    else if (this->IsReal() && oper.IsString()) // Oper1 is double and Oper2 is string
    {
        //first operand must be converted to a string
        str1 << this->GetReal(); //may throw invalid_argument()

        string string1 = str1.str();
        string string2 = oper.GetString();
        status = string1 < string2;
        return Value(status);
    }
    else if (this->IsString() && oper.IsReal()) // Oper1 is string and Oper2 is double
    {
        //second operand must be converted to a string
        str2 << oper.GetReal(); //may throw invalid_argument()     

        string string1 = this->GetString();
        string string2 = str2.str();
        status = string1 < string2;
        return Value(status);
    }
    else if (this->IsErr() || oper.IsErr()) // Checking if either operator is an error
    {
        return Value();
    }
    else if (this->IsString() && oper.IsString()) // Both operators are strings
    {
        string string1, string2;
        string1 = this->GetString();
        string2 = oper.GetString();
        status = string1 < string2;
        return Value(status);
    }

    return Value();
     
}

/*
	//Numeric exponentiation operation this raised to the power of op 
	Value operator^(const Value& oper) const;
    
    
    
	//string less than operator of this with op
	Value SLthan(const Value& oper) const;*/


