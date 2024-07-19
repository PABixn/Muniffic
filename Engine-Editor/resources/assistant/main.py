import os
import time

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
        tools=[{"type": "file_search"}],
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
        elif run.status == "requires_action":
            return run.status
        elif run.status == "cancelled":
            return run.status
        elif run.status == "expired":
            return run.status

        time.sleep(1)


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
