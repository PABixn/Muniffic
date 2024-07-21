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

                    Console.Write("Enter parameter type (string, number, boolean, object, array): ");
                    string paramType = Console.ReadLine();

                    Console.Write("Enter parameter description (optional): ");
                    string paramDescription = Console.ReadLine();

                    Console.Write("Is this parameter required? (y/n): ");
                    string isRequired = Console.ReadLine();
                    bool required = isRequired.ToLower() == "y";

                    var parameterDetails = new Dictionary<string, string>
                    {
                        { "type", paramType }
                    };

                    if (!string.IsNullOrEmpty(paramDescription))
                    {
                        parameterDetails.Add("description", paramDescription);
                    }

                    properties[paramName] = parameterDetails;

                    if (required)
                    {
                        requiredParams.Add(paramName);
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
                            required = requiredParams.Count > 0 ? requiredParams : new List<string>()
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
