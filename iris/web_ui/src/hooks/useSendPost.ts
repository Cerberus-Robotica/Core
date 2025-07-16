import axios from 'axios';

export async function sendPost(url: string, data: Record<string, unknown>): Promise<boolean> {
  try {
    const res = await axios.post(url, data, {
      headers: { 'Content-Type': 'application/json' },
    });
    return res.status === 200;
  } catch (error) {
    console.error('Erro ao enviar POST:', error);
    return false;
  }
}
