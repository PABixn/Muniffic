import json
import os
import time
from typing import Dict, Any

import openai
from dotenv import load_dotenv


def create_assistant(name, model="gpt-4o-mini", instructions=None):
    load_dotenv()
    client = openai.OpenAI()

    vector_store = client.beta.vector_stores.create(name="Muniffic documentation")

    file_paths = [os.getcwd() + "/resources/assistant/doc/Muniffic.docx",
                  os.getcwd() + "/resources/assistant/doc/Scripting Engine Docs/Scripting-Engine.cs",
                  os.getcwd() + "/resources/assistant/doc/Scripting Engine Docs/Scripting-Engine-Examples.cs"]

    file_streams = [open(path, "rb") for path in file_paths]

    file_batch = client.beta.vector_stores.file_batches.upload_and_poll(vector_store_id=vector_store.id, files=file_streams)

    print(file_batch.status)

    assistant = client.beta.assistants.create(
        name=name,
        model=model,
        instructions=instructions,
        tools=[
            {"type": "file_search"},
            {
                "type": "function",
                "function": {
                    "name": "LoadEntitySystemFunctions",
                    "description": "Loads all functions for the entity system.",
                    "parameters": {
                        "type": "object",
                        "properties": {},
                        "required": []
                    }
                }
            }
        ],
        tool_resources={"file_search": {"vector_store_ids": [vector_store.id]}})

    return assistant.id


def create_thread():
    load_dotenv()
    client = openai.OpenAI()
    thread = client.beta.threads.create()
    return thread.id


def add_message(thread_id, message):
    load_dotenv()
    client = openai.OpenAI()
    client.beta.threads.messages.create(
        thread_id=thread_id,
        role="user",
        content=message)


def initiate_run(assistant_id, thread_id, instructions=None):
    load_dotenv()
    client = openai.OpenAI()
    run = client.beta.threads.runs.create(
        assistant_id=assistant_id,
        thread_id=thread_id,
        instructions=instructions)
    return run.id


def wait_for_completion(thread_id, run_id):
    is_first_stage = True
    load_dotenv()
    client = openai.OpenAI()
    while True:
        run = client.beta.threads.runs.retrieve(
            thread_id=thread_id,
            run_id=run_id)

        if run.status == "completed":
            return run.status
        elif run.status == "failed":
            return run.status
        elif run.status == "requires_action" and is_first_stage:
            is_first_stage = False
            take_required_action(thread_id, run_id)
        elif run.status == "requires_action" and not is_first_stage:
            return run.status
        elif run.status == "cancelled":
            return run.status
        elif run.status == "expired":
            return run.status

        time.sleep(1)


def get_tool_call(thread_id, run_id):
    result = []

    load_dotenv()
    client = openai.OpenAI()

    run = client.beta.threads.runs.retrieve(
        thread_id=thread_id,
        run_id=run_id)

    for tool in run.required_action.submit_tool_outputs.tool_calls:
        resulted = {"tool_call_id": tool.id, "function_name": tool.function.name}

        params = json.loads(tool.function.arguments)

        i = 0
        for key, value in params.items():
            resulted["arg%d" % i] = value
            i += 1

        result.append(resulted)
        
    return result


def submit_tool_outputs(thread_id, run_id, output):
    load_dotenv()
    client = openai.OpenAI()
    res = json.loads(output)
    tool_output = []

    for key, value in res.items():
        tool_output.append({"tool_call_id": key, "output": value})

    try:
        run = client.beta.threads.runs.submit_tool_outputs_and_poll(
            thread_id=thread_id,
            run_id=run_id,
            tool_outputs=tool_output
        )
        print("Tool outputs submitted successfully.")
    except Exception as e:
        print("Failed to submit tool outputs:", e)


def take_required_action(thread_id, run_id):
    load_dotenv()
    client = openai.OpenAI()

    tool_outputs = []

    run = client.beta.threads.runs.retrieve(
        thread_id=thread_id,
        run_id=run_id)

    for tool in run.required_action.submit_tool_outputs.tool_calls:
        if tool.function.name == "LoadEntitySystemFunctions":
            output = load_entity_system_functions()
            tool_outputs.append({"tool_call_id": tool.id, "output": output})
        else:
            print(tool.json)

    if tool_outputs:
        try:
            run = client.beta.threads.runs.submit_tool_outputs_and_poll(
                thread_id=thread_id,
                run_id=run_id,
                tool_outputs=tool_outputs
            )
            print("Tool outputs submitted successfully.")
        except Exception as e:
            print("Failed to submit tool outputs:", e)
    else:
        print("No tool outputs to submit.")


def load_entity_system_functions():
    file = open(os.getcwd() + "/resources/assistant/doc/EntitySystemFunctions.txt", "r")
    return file.read()


def get_run_status(thread_id, run_id):
    load_dotenv()
    client = openai.OpenAI()
    run = client.beta.threads.runs.retrieve(
        thread_id=thread_id,
        run_id=run_id)
    return run.status


def get_last_message(thread_id):
    load_dotenv()
    client = openai.OpenAI()
    response = client.beta.threads.messages.list(thread_id=thread_id).data[0].content[0].text.value
    return response


def check_if_assistant_exists(assistant_id):
    load_dotenv()
    client = openai.OpenAI()
    try:
        assistant = client.beta.assistants.retrieve(assistant_id=assistant_id)
        return True
    except Exception:
        return False
