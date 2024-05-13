import os
import time

import openai
from dotenv import load_dotenv


def create_assistant(name, model="gpt-3.5-turbo", instructions=None, tools=None):
    load_dotenv()
    client = openai.OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
    assistant = client.beta.assistants.create(
        name=name,
        model=model,
        instructions=instructions,
        tools=tools)
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
    print(assistant_id, thread_id, instructions)
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


def get_last_message(thread_id):
    load_dotenv()
    client = openai.OpenAI()
    response = client.beta.threads.messages.list(thread_id=thread_id).data[0].content[0].text.value
    return response
