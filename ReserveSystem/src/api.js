import axios from "axios";

class ReserveApi {
    static async reserveCar(carId){
        try{
            const body = {
                car_id : carId,
            }
            const response = await axios.post(
                "http://192.168.1.5/reserve/",
                body
            )
            if(response) return response;

        }catch (e) {
            if(e.response) return e.response;
        }
    }
}

export default ReserveApi;
