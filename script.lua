function PrintPerson(person)
    print("PERSON: " .. person.name .. ", " .. person.age)
end

function GetPersonData(person)
    return person.name, person.age
end

Bjarne = { name = "Bjarne Stroustrup", age = 73 }
Rich = { name = "Rich Hickey", age = 55 }
Name, Age = GetPersonData(Bjarne)
--Name, Age = GetPersonData { name = "Rich Hickey", age = 55 }

print "Script.lua successfully loaded. Printing person:"
PrintPerson { name = "Rich Hickey", age = 42 }
