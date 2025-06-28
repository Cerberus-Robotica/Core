import axios from 'axios';
import type { DataType } from '../types';

export async function sendData(data: Partial<DataType>): Promise<void> {
  try {
    await axios.post('http://localhost:5000/data', data);
  } catch (error) {
    console.error('Erro ao enviar dados:', error);
    throw error;
  }
}
