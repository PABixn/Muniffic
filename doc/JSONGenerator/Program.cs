using System;
using System.Collections.Generic;
using System.Text.Json;

namespace OpenAIFunctionDefGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("OpenAI Function Definition Generator");
            Console.WriteLine("-------------------------------------");

            var functionDefinitions = new List<object>();
            bool addMoreFunctions = true;

            while (addMoreFunctions)
            {
                // Prompt for function name
                Console.Write("Enter the function name: ");
                string functionName = Console.ReadLine();

                // Prompt for function description
                Console.Write("Enter the function description: ");
                string functionDescription = Console.ReadLine();

                // Prompt for parameters
                var properties = new Dictionary<string, object>();
                var requiredParams = new List<string>();
                bool addMoreParameters = true;

                while (addMoreParameters)
                {
                    Console.Write("Enter parameter name (or press Enter to finish): ");
                    string paramName = Console.ReadLine();
                    if (string.IsNullOrEmpty(paramName))
                    {
                        addMoreParameters = false;
                        continue;
                    }

                    Console.Write("Is this parameter required? (y/n): ");
                    string isRequired = Console.ReadLine();
                    bool required = isRequired.ToLower() == "y";

                    if (required)
                    {
                        requiredParams.Add(paramName);
                    }

                    Console.Write("Does this parameter have oneOf conditions? (y/n): ");
                    string hasOneOf = Console.ReadLine();
                    if (hasOneOf.ToLower() == "y")
                    {
                        var oneOfTypeList = new List<Dictionary<string, object>>();
                        bool addMoreOneOf = true;
                        while (addMoreOneOf)
                        {
                            Console.Write("Enter oneOf parameter type (string, number, boolean, object, array): ");
                            string oneOfType = Console.ReadLine();

                            Console.Write("Enter oneOf parameter description (optional): ");
                            string oneOfDescription = Console.ReadLine();

                            var oneOfDetails = new Dictionary<string, object>
                            {
                                { "type", oneOfType }
                            };

                            if (!string.IsNullOrEmpty(oneOfDescription))
                            {
                                oneOfDetails.Add("description", oneOfDescription);
                            }

                            Console.Write("Does this oneOf parameter have restricted values (enum)? (y/n): ");
                            string hasOneOfEnum = Console.ReadLine();
                            if (hasOneOfEnum.ToLower() == "y")
                            {
                                var oneOfEnumValues = new List<string>();
                                bool addMoreOneOfEnumValues = true;
                                while (addMoreOneOfEnumValues)
                                {
                                    Console.Write("Enter an allowed value: ");
                                    string oneOfEnumValue = Console.ReadLine();
                                    oneOfEnumValues.Add(oneOfEnumValue);

                                    Console.Write("Add another allowed value? (y/n): ");
                                    string addAnotherOneOfEnumValue = Console.ReadLine();
                                    addMoreOneOfEnumValues = addAnotherOneOfEnumValue.ToLower() == "y";
                                }
                                oneOfDetails.Add("enum", oneOfEnumValues);
                            }

                            oneOfTypeList.Add(oneOfDetails);

                            Console.Write("Add another oneOf type for this parameter? (y/n): ");
                            string addAnotherOneOf = Console.ReadLine();
                            addMoreOneOf = addAnotherOneOf.ToLower() == "y";
                        }
                        properties[paramName] = new { oneOf = oneOfTypeList };
                    }
                    else
                    {
                        Console.Write("Enter parameter type (string, number, boolean, object, array): ");
                        string paramType = Console.ReadLine();

                        Console.Write("Enter parameter description (optional): ");
                        string paramDescription = Console.ReadLine();

                        var parameterDetails = new Dictionary<string, object>
                        {
                            { "type", paramType }
                        };

                        if (!string.IsNullOrEmpty(paramDescription))
                        {
                            parameterDetails.Add("description", paramDescription);
                        }

                        Console.Write("Does this parameter have restricted values (enum)? (y/n): ");
                        string hasEnum = Console.ReadLine();
                        if (hasEnum.ToLower() == "y")
                        {
                            var enumValues = new List<string>();
                            bool addMoreEnumValues = true;
                            while (addMoreEnumValues)
                            {
                                Console.Write("Enter an allowed value: ");
                                string enumValue = Console.ReadLine();
                                enumValues.Add(enumValue);

                                Console.Write("Add another allowed value? (y/n): ");
                                string addAnotherEnumValue = Console.ReadLine();
                                addMoreEnumValues = addAnotherEnumValue.ToLower() == "y";
                            }
                            parameterDetails.Add("enum", enumValues);
                        }

                        Console.Write("Enter an example value (optional): ");
                        string exampleValue = Console.ReadLine();
                        if (!string.IsNullOrEmpty(exampleValue))
                        {
                            parameterDetails.Add("example", exampleValue);
                        }

                        properties[paramName] = parameterDetails;
                    }
                }

                // Create function definition
                var functionDefinition = new
                {
                    type = "function",
                    function = new
                    {
                        name = functionName,
                        description = functionDescription,
                        parameters = new
                        {
                            type = "object",
                            properties = properties,
                            required = requiredParams.Count > 0 ? requiredParams : new List<string>(),
                        }
                    }
                };

                // Add to the list of function definitions
                functionDefinitions.Add(functionDefinition);

                // Prompt to add another function
                Console.Write("Do you want to add another function? (y/n): ");
                string addAnotherFunction = Console.ReadLine();
                addMoreFunctions = addAnotherFunction.ToLower() == "y";
            }

            // Convert the list of function definitions to JSON
            var options = new JsonSerializerOptions { WriteIndented = true };
            string jsonString = JsonSerializer.Serialize(functionDefinitions, options);

            // Output JSON
            Console.WriteLine("\nGenerated JSON Function Definitions:");
            Console.WriteLine(jsonString);
        }
    }
}